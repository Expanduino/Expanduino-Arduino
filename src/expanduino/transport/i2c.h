#pragma once

#include "../expanduino.h"

class PrintBuffer : public Print {
public:
  union {
    uint8_t len;
    uint8_t data[EXPANDUINO_MAX_RESPONSE_SIZE];
  } buffer;
  virtual size_t write(uint8_t);
};

class ExpanduinoI2C_ : public Expanduino {
  int interruptPin;
public:
  void begin(int address, int interruptPin);
  void end();
  
  virtual void requestInterrupt();
  virtual bool clearInterruptStatus();
  
  static void wireOnReceive(int howMany);
  static void wireOnRequest();
};

extern ExpanduinoI2C_ ExpanduinoI2C;
