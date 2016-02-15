#pragma once

#include "../expanduino.h"

enum ExpanduinoCommandSerial {
  EXPANDUINO_CMD_SERIAL_NUM_SERIALS  = 0,
  EXPANDUINO_CMD_SERIAL_NAME         = 1,
  EXPANDUINO_CMD_SERIAL_WRITE        = 2,
  EXPANDUINO_CMD_SERIAL_READ         = 3,
  EXPANDUINO_CMD_SERIAL_AVAILABLE    = 4
};

class ExpanduinoSubdeviceSerial : public ExpanduinoSubdevice {
  uint8_t nextInterruptFrom;
public:
  ExpanduinoSubdeviceSerial(Expanduino& container, const char* name="Serial", const char* shortName="serial"); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
  virtual void readInterruptionData(Print& response);
  virtual void getSerialName(uint8_t serialNum, Print& name);
  
  virtual uint8_t getNumSerials() = 0;
  virtual void write(uint8_t serialNum, Stream& data, uint8_t dataLen) = 0;
  virtual void read(uint8_t serialNum, Print& data, uint8_t dataLen) = 0;
  virtual uint8_t available(uint8_t serialNum) = 0;
};
