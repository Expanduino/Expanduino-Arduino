#pragma once

#include "../classes/serial.h"
#include <Arduino.h>

class ExpanduinoSubdeviceHardwareSerialArduino : public ExpanduinoSubdeviceSerial {
  HardwareSerial** serials;
  int numSerials;
public:
  ExpanduinoSubdeviceHardwareSerialArduino(Expanduino& container, const char* name, const char* shortName, HardwareSerial** serials, int numSerials);
  virtual void begin();
  virtual void end();
  virtual uint8_t getNumSerials();
  virtual void write(uint8_t serialNum, Stream& data, uint8_t dataLen);
  virtual void read(uint8_t serialNum, Print& data, uint8_t dataLen);
  virtual uint8_t available(uint8_t serialNum);
};
