#pragma once

#include "../classes/serial.h"
#include <LoopbackStream.h>
#include <Wiegand.h>



typedef struct {
  int pin0, pin1;
  LoopbackStream buffer;
  Wiegand wiegand;
} ArduinoWiegandComponent;

class ExpanduinoSubdeviceWiegandSerialArduino : public ExpanduinoSubdeviceSerial, public Runnable {
  ArduinoWiegandComponent* wiegands;
  int numWiegands;
public:
  ExpanduinoSubdeviceWiegandSerialArduino(Expanduino& container, const char* name, const char* shortName, ArduinoWiegandComponent* wiegands, int numWiegands);
  virtual void begin();
  virtual void end();
  virtual void run();
  virtual uint8_t getNumSerials();
  virtual void write(uint8_t serialNum, Stream& data, uint8_t dataLen);
  virtual void read(uint8_t serialNum, Print& data, uint8_t dataLen);
  virtual uint8_t available(uint8_t serialNum);
};
