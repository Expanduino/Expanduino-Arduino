#include "gpio-linux-input.h"
#include <Arduino.h>
#include <YetAnotherPcInt.h>

static void pinChanged(ExpanduinoSubdeviceGpioLinuxInputArduino* listener, bool pinstate) {
  listener->requestInterruption();
}

ExpanduinoSubdeviceGpioLinuxInputArduino::ExpanduinoSubdeviceGpioLinuxInputArduino(Expanduino& container, const char* name, const char* shortName, const LinuxInputId &linuxInputId, ArduinoGpioLinuxInputComponent* components, int numComponents) 
: ExpanduinoSubdeviceLinuxInput(container, name, shortName),
  linuxInputId(linuxInputId),
  components(components),
  numComponents(numComponents)
{
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::begin() {
  for (int i=0; i<numComponents; i++) {
    components[i].value = 0;
    switch (components[i].type.type) {
      case EV_KEY:
      case EV_SW: {
        pinMode(components[i].pin, INPUT_PULLUP);
        if (digitalPinToPCICR(components[i].pin)) {
          PcInt::attachInterrupt(components[i].pin, pinChanged, this, CHANGE, true);
        }
        break;
      }
      case EV_ABS: {
        pinMode(components[i].pin, INPUT);
        break;
      }
      case EV_LED:
      case EV_SND: {
        pinMode(components[i].pin, OUTPUT);
        digitalWrite(components[i].pin, LOW);
        break;
      }
    }
    setValue(i, 0);
  }
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::end() {
  for (int i=0; i<numComponents; i++) {
    components[i].value = 0;
    switch (components[i].type.type) {
      case EV_KEY:
      case EV_SW: 
      case EV_LED:
        if (digitalPinToPCICR(components[i].pin)) {
          PcInt::detachInterrupt(components[i].pin);
        }
      case EV_SND: 
      case EV_ABS: {
        pinMode(components[i].pin, INPUT);
        break;
      }
    }
    setValue(i, 0);
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
  ArduinoGpioLinuxInputComponent& comp = components[componentNum];
  switch (comp.type.type) {
    case EV_KEY:
    case EV_SW: {
      comp.value = digitalRead(comp.pin) ? 0 : 1; //EV_KEY, EV_SW are Active-low
      break;
    }
    case EV_ABS: {
      comp.value = analogRead(comp.pin);
      break;
    }
  }
  return comp.value;
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::setValue(uint8_t componentNum, int32_t value) {
  ArduinoGpioLinuxInputComponent& comp = components[componentNum];
  
  switch (comp.type.type) {
    
    case EV_LED: {
      value = value ? 1 : 0;
      digitalWrite(comp.pin, value ? HIGH : LOW);
      break;
    }
    
    //pwm-beeper.c
    case EV_SND: {
      if (comp.type.code == SND_TONE) {
        if (value > 0) {
          tone(comp.pin, value);
        } else {
          noTone(comp.pin);
        }
      } else if (comp.type.code == SND_BELL) {
        if (value > 0) {
          analogWrite(comp.pin, 127);
        } else {
          analogWrite(comp.pin, 0);
        }
      }
    }
  }
  
  comp.value = value;
}

void ExpanduinoSubdeviceGpioLinuxInputArduino::getInterruptionReport(LinuxInputStateChange* &valuesArray, uint8_t &valuesArrayLength) {
  valuesArray = (LinuxInputStateChange*)malloc(this->getNumComponents() * sizeof(LinuxInputStateChange));
  for (uint8_t i=0; i<this->getNumComponents(); i++) {
    int32_t old = this->components[i].value;
    int32_t current = this->getValue(i);
    if (old != current) {
      valuesArray[valuesArrayLength].component = i;
      valuesArray[valuesArrayLength].value = current;
      valuesArrayLength++;
    }
  }
}