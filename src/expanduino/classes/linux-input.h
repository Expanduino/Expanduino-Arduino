#pragma once

#include "../expanduino.h"
#include "linux-input-event-codes.h"

enum ExpanduinoCommandLinuxInput {
  EXPANDUINO_CMD_LINUX_INPUT_ID                   = 0,
  EXPANDUINO_CMD_LINUX_INPUT_NUM_COMPONENTS       = 1,
  EXPANDUINO_CMD_LINUX_INPUT_COMPONENT_TYPE       = 2,
  EXPANDUINO_CMD_LINUX_INPUT_COMPONENT_ABS_INFO   = 3,
  EXPANDUINO_CMD_LINUX_INPUT_GET_COMPONENT_VALUE  = 4,
  EXPANDUINO_CMD_LINUX_INPUT_SET_COMPONENT_VALUE  = 5
};

class LinuxInputId {
public:
  uint16_t vendor;
  uint16_t product;
  uint16_t version;
};

class LinuxInputComponentType {
public:
  uint16_t type;
  uint16_t code;
};

class LinuxInputAbsoluteComponentConfig {
public:
  int32_t max;
  int32_t min;
  int32_t fuzz;
  int32_t flat;
};

#define LINUX_INPUT_MAX_INTERRUPT_EVENTS 6
class LinuxInputStateChange {
  public:
  uint8_t component;
  int32_t value;
};

class ExpanduinoSubdeviceLinuxInput : public ExpanduinoSubdevice {
public:
  ExpanduinoSubdeviceLinuxInput(Expanduino& container, const char* name="Linux Input", const char* shortName="linux-input"); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
  virtual void reset();
  virtual const LinuxInputId& getLinuxInputId() = 0;
  virtual uint8_t getNumComponents() = 0;
  virtual LinuxInputComponentType getComponentType(uint8_t componentNum) = 0;
  virtual LinuxInputAbsoluteComponentConfig getAbsoluteComponentConfig(uint8_t componentNum) = 0;
  virtual int32_t getValue(uint8_t componentNum) = 0;
  virtual void setValue(uint8_t componentNum, int32_t value) = 0;
  
  virtual void readInterruptionData(Print& response);
  virtual void getInterruptionReport(LinuxInputStateChange* valuesArray, uint8_t &valuesArrayLength) = 0;
};
