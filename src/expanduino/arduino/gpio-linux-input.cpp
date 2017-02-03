#include "gpio-linux-input.h"
#include <Arduino.h>
#include <YetAnotherPcInt.h>

#define DEBOUNCE 5

static void ArduinoGpioLinuxInputComponent::pcint(ArduinoGpioLinuxInputComponent* _this, bool value) {
  int delay = _this->debouncer.set_value(value, false);
  scheduler.removeCallbacks(_this);
  if (delay >= 0) {
      scheduler.scheduleDelayed(_this, delay);
  }
}

void ArduinoGpioLinuxInputComponent::run() {
  //The debouncer requires external synchronization so that we don't
  //call debouncer.flush() and debouncer.set_value() at the same time,
  noInterrupts();
  int delay = debouncer.flush();
  interrupts();
  if (delay >= 0) {
      scheduler.scheduleDelayed(this, delay);
  }
}

static void ArduinoGpioLinuxInputComponent::changed(bool value, ArduinoGpioLinuxInputComponent* _this) {
  //This is called from debouncer.flush(), but no synchronization is required here
//   interrupts();
  _this->changed(value);
//   noInterrupts();
}

void ArduinoGpioLinuxInputComponent::changed(bool value) {
  int id = this - this->parent->components;
  this->parent->event_buffer.write( (id << 1) | (value ? 0 : 1));
  this->parent->requestInterruption();
}





ArduinoGpioLinuxInputComponent::ArduinoGpioLinuxInputComponent(int pin, const LinuxInputComponentType &type) {
  this->pin = pin;
  this->type = type;
}

void ArduinoGpioLinuxInputComponent::begin() {
  switch (this->type.type) {
    case EV_KEY:
    case EV_SW: {
      pinMode(this->pin, INPUT_PULLUP);

      if (digitalPinToPCICR(this->pin)) {
        this->debouncer.begin(digitalRead(this->pin));
        this->debouncer.set_debounce_stable(DEBOUNCE);
        this->debouncer.set_callback(ArduinoGpioLinuxInputComponent::changed, this);

        PcInt::attachInterrupt(this->pin, ArduinoGpioLinuxInputComponent::pcint, this, CHANGE, true);
      }
      break;
    }
    case EV_ABS: {
      pinMode(this->pin, INPUT);
      break;
    }
    case EV_LED:
    case EV_SND: {
      pinMode(this->pin, OUTPUT);
      digitalWrite(this->pin, LOW);
      break;
    }
  }
  this->setValue(0);
}


void ArduinoGpioLinuxInputComponent::end() {
  switch (this->type.type) {
    case EV_KEY:
    case EV_SW:
      if (digitalPinToPCICR(this->pin)) {
        PcInt::detachInterrupt(this->pin);

        this->debouncer.end();
      }

    case EV_ABS:
    case EV_LED:
    case EV_SND: {
      pinMode(this->pin, INPUT);
      break;
    }
  }
  this->setValue(0);
}


int32_t ArduinoGpioLinuxInputComponent::getValue() {
  switch (this->type.type) {
    case EV_KEY:
    case EV_SW: {
      if (digitalPinToPCICR(this->pin)) {
        this->value = this->debouncer.get_stable_value() ? 0 : 1;
      } else {
        this->value = digitalRead(this->pin) ? 0 : 1; //EV_KEY, EV_SW are Active-low
      }
      break;
    }
    case EV_ABS: {
      this->value = analogRead(this->pin);
      break;
    }
  }
  return this->value;
}

void ArduinoGpioLinuxInputComponent::setValue(int32_t value) {
  switch (this->type.type) {

    case EV_LED: {
      value = value ? 1 : 0;
      digitalWrite(this->pin, value ? HIGH : LOW);
      break;
    }

    //pwm-beeper.c
    case EV_SND: {
      if (this->type.code == SND_TONE) {
        digitalWrite(this->pin, value ? HIGH : LOW);
        /*if (value > 0) {
          tone(this->pin, value);
        } else {
          noTone(this->pin);
        }*/
      } else if (this->type.code == SND_BELL) {
        digitalWrite(this->pin, value ? HIGH : LOW);
        /*
        if (value > 0) {
          analogWrite(this->pin, 127);
        } else {
          analogWrite(this->pin, 0);
        }*/
      }
    }
  }

  this->value = value;
}










ExpanduinoSubdeviceGpioLinuxInputArduino::ExpanduinoSubdeviceGpioLinuxInputArduino(Expanduino& container, const char* name, const char* shortName, const LinuxInputId &linuxInputId, ArduinoGpioLinuxInputComponent* components, int numComponents) 
: ExpanduinoSubdeviceLinuxInput(container, name, shortName),
  linuxInputId(linuxInputId),
  components(components),
  numComponents(numComponents)
{
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::begin() {
  this->event_buffer.reset();
  for (int i=0; i<numComponents; i++) {
    components[i].parent = this;
    components[i].begin();
  }
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::end() {
  for (int i=0; i<numComponents; i++) {
    components[i].end();
  }
}

const LinuxInputId& ExpanduinoSubdeviceGpioLinuxInputArduino::getLinuxInputId() {
  return linuxInputId;
}
uint8_t ExpanduinoSubdeviceGpioLinuxInputArduino::getNumComponents() {
  return numComponents;
}
LinuxInputComponentType ExpanduinoSubdeviceGpioLinuxInputArduino::getComponentType(uint8_t componentNum) {
  return components[componentNum].type;
}

LinuxInputAbsoluteComponentConfig ExpanduinoSubdeviceGpioLinuxInputArduino::getAbsoluteComponentConfig(uint8_t componentNum) {
  LinuxInputAbsoluteComponentConfig ret = {
    .max=1023, //Default resolution on arduinos. 
    .min=0,
    .fuzz=0,
    .flat=0
  };
  return ret;
}

int32_t ExpanduinoSubdeviceGpioLinuxInputArduino::getValue(uint8_t componentNum) {
  return components[componentNum].getValue();
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::setValue(uint8_t componentNum, int32_t value) {
  components[componentNum].setValue(value);
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::getInterruptionReport(LinuxInputStateChange* valuesArray, uint8_t &valuesArrayLength) {
  // Sends events in the queue
  while ( (this->event_buffer.available())  && (valuesArrayLength < LINUX_INPUT_MAX_INTERRUPT_EVENTS) ) {
    uint8_t event = this->event_buffer.read();
    uint8_t i = event >> 1;
    int32_t newValue = event & 1;

    if (this->components[i].value != newValue) {
      this->components[i].value = newValue;

      valuesArray[valuesArrayLength].component = i;
      valuesArray[valuesArrayLength].value = newValue;
      valuesArrayLength++;
    }
  }

  // Send any other inconsistent values too
  for (uint8_t i=0; (i<this->getNumComponents()) && (valuesArrayLength < LINUX_INPUT_MAX_INTERRUPT_EVENTS); i++) {
    int32_t oldValue = this->components[i].value;
    int32_t newValue = this->getValue(i);
    if (oldValue != newValue) {
      valuesArray[valuesArrayLength].component = i;
      valuesArray[valuesArrayLength].value = newValue;
      valuesArrayLength++;
    }
  }

  if (valuesArrayLength == LINUX_INPUT_MAX_INTERRUPT_EVENTS) {
    this->requestInterruption();
  }

}
