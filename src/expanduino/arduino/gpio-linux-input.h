#pragma once

#include "../classes/linux-input.h"

typedef struct {
  int pin;
  LinuxInputComponentType type;
  int32_t value;
} ArduinoGpioLinuxInputComponent;

class ExpanduinoSubdeviceGpioLinuxInputArduino : public ExpanduinoSubdeviceLinuxInput {
  LinuxInputId linuxInputId;
  int numComponents;
  ArduinoGpioLinuxInputComponent* components;
public:
  ExpanduinoSubdeviceGpioLinuxInputArduino(Expanduino& container, const LinuxInputId &linuxInputId, int numComponents, ArduinoGpioLinuxInputComponent* components);
  virtual void begin();
  virtual void end();
  virtual const LinuxInputId& getLinuxInputId();
  virtual uint8_t getNumComponents();
  virtual LinuxInputComponentType getComponentType(uint8_t componentNum);
  virtual LinuxInputAbsoluteComponentConfig getAbsoluteComponentConfig(uint8_t componentNum);
  virtual int32_t getValue(uint8_t componentNum);
  virtual void setValue(uint8_t componentNum, int32_t value);
  virtual void getInterruptionReport(LinuxInputStateChange* &valuesArray, uint8_t &valuesArrayLength);
};
