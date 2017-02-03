#pragma once

#include "../classes/linux-input.h"
#include <YetAnotherDebouncer.h>
#include <LoopbackStream.h>

class ExpanduinoSubdeviceGpioLinuxInputArduino;

class ArduinoGpioLinuxInputComponent : public Runnable {
public:
  int pin;
  LinuxInputComponentType type;
  int32_t value;
  ExpanduinoSubdeviceGpioLinuxInputArduino* parent;
  Debouncer<Clock::Millis> debouncer;

  ArduinoGpioLinuxInputComponent(int pin, const LinuxInputComponentType &type);
  static void pcint(ArduinoGpioLinuxInputComponent* _this, bool value);
  static void changed(bool value, ArduinoGpioLinuxInputComponent* _this);
  void changed(bool value);
  virtual void run();
  void begin();
  void end();
  int32_t getValue();
  void setValue(int32_t v);
};

class ExpanduinoSubdeviceGpioLinuxInputArduino : public ExpanduinoSubdeviceLinuxInput {
  LinuxInputId linuxInputId;
  int numComponents;
  ArduinoGpioLinuxInputComponent* components;
  LoopbackStream event_buffer;

  friend class ArduinoGpioLinuxInputComponent;
public:
  ExpanduinoSubdeviceGpioLinuxInputArduino(Expanduino& container, const char* name, const char* shortName, const LinuxInputId &linuxInputId, ArduinoGpioLinuxInputComponent* components, int numComponents);
  virtual void begin();
  virtual void end();
  virtual const LinuxInputId& getLinuxInputId();
  virtual uint8_t getNumComponents();
  virtual LinuxInputComponentType getComponentType(uint8_t componentNum);
  virtual LinuxInputAbsoluteComponentConfig getAbsoluteComponentConfig(uint8_t componentNum);
  virtual int32_t getValue(uint8_t componentNum);
  virtual void setValue(uint8_t componentNum, int32_t value);
  virtual void getInterruptionReport(LinuxInputStateChange* valuesArray, uint8_t &valuesArrayLength);
};
