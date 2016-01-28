#pragma once

#include "../expanduino.h"
#include "linux-input-event-codes.h"

enum ExpanduinoCommandLinuxInput {
  EXPANDUINO_CMD_LINUX_INPUT_ID                  = 0,
  EXPANDUINO_CMD_LINUX_INPUT_NUM_COMPONENTS      = 1,
  EXPANDUINO_CMD_LINUX_INPUT_GET_COMPONENT_TYPE  = 2,
  EXPANDUINO_CMD_LINUX_INPUT_GET_COMPONENT_VALUE = 3,
  EXPANDUINO_CMD_LINUX_INPUT_SET_COMPONENT_VALUE = 4
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

class ExpanduinoSubdeviceLinuxInput : public ExpanduinoSubdevice {
public:
  ExpanduinoSubdeviceLinuxInput(Expanduino& container, const char* name="Linux Input", const char* shortName="linux-input"); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
  virtual void reset();
  virtual const LinuxInputId& getLinuxInputId() = 0;
  virtual uint8_t getNumComponents() = 0;
  virtual const LinuxInputComponentType& getComponentType(uint8_t componentNum) = 0;
  virtual int32_t getValue(uint8_t componentNum) = 0;
  virtual void setValue(uint8_t componentNum, int32_t value) = 0;
//   virtual void notifyChange(uint8_t componentNum, int32_t value) = 0;
};
