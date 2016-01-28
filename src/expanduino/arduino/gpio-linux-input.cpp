#include "gpio-linux-input.h"
#include <Arduino.h>

ExpanduinoSubdeviceGpioLinuxInputArduino::ExpanduinoSubdeviceGpioLinuxInputArduino(Expanduino& container, const LinuxInputId &linuxInputId, int numComponents, ArduinoGpioLinuxInputComponent* components) 
: ExpanduinoSubdeviceLinuxInput(container),
  linuxInputId(linuxInputId),
  numComponents(numComponents),
  components(components)
{
  for (int i=0; i<numComponents; i++) {
    components[i].value = 0;
    switch (components[i].type.type) {
      case EV_KEY:
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

const LinuxInputId& ExpanduinoSubdeviceGpioLinuxInputArduino::getLinuxInputId() {
  return linuxInputId;
}
uint8_t ExpanduinoSubdeviceGpioLinuxInputArduino::getNumComponents() {
  return numComponents;
}
const LinuxInputComponentType& ExpanduinoSubdeviceGpioLinuxInputArduino::getComponentType(uint8_t componentNum) {
  return components[componentNum].type;
}

int32_t ExpanduinoSubdeviceGpioLinuxInputArduino::getValue(uint8_t componentNum) {
  ArduinoGpioLinuxInputComponent& comp = components[componentNum];
  switch (comp.type.type) {
    case EV_KEY: {
      comp.value = digitalRead(comp.pin) ? 1 : 0;
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
  Serial.print("set input ");
  Serial.print(comp.type.type);
  
  switch (comp.type.type) {
    
    case EV_LED: {
      Serial.print(" led! ");
      value = value ? 1 : 0;
      digitalWrite(comp.pin, value ? HIGH : LOW);
      break;
    }
    
    //pwm-beeper.c
    case EV_SND: {
      Serial.print(" SND! ");
      if (comp.type.code == SND_TONE) {
        if (value > 0) {
          tone(comp.pin, value);
        } else {
          noTone(comp.pin);
        }
      } else if (comp.type.code == SND_BELL) {
        if (value > 0) {
          tone(comp.pin, 1000);
        } else {
          noTone(comp.pin);
        }
      }
    }
  }
  
  comp.value = value;
}