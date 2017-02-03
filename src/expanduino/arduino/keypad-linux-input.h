#pragma once

#include "../classes/linux-input.h"

typedef struct {
  LinuxInputComponentType type;
  bool actualValue;
  bool knownValue;
} ArduinoKeypadLinuxInputComponent;

class ExpanduinoSubdeviceKeypadLinuxInputArduino : public ExpanduinoSubdeviceLinuxInput, public Runnable {
  LinuxInputId linuxInputId;
  int* rows;
  int numRows;
  int* cols;
  int numCols;
  ArduinoKeypadLinuxInputComponent* components;
  uint8_t currentRow;
  bool rowEnabled;
public:
  ExpanduinoSubdeviceKeypadLinuxInputArduino(Expanduino& container, const char* name, const char* shortName, const LinuxInputId &linuxInputId, int* rows, int numRows, int* cols, int numCols, ArduinoKeypadLinuxInputComponent* components);
  virtual void begin();
  virtual void end();
  virtual void run();
  virtual const LinuxInputId& getLinuxInputId();
  virtual uint8_t getNumComponents();
  virtual LinuxInputComponentType getComponentType(uint8_t componentNum);
  virtual LinuxInputAbsoluteComponentConfig getAbsoluteComponentConfig(uint8_t componentNum);
  virtual int32_t getValue(uint8_t componentNum);
  virtual void setValue(uint8_t componentNum, int32_t value);
  virtual void getInterruptionReport(LinuxInputStateChange* &valuesArray, uint8_t &valuesArrayLength);
};
