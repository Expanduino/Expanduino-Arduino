#include "i2c.h"
#include <Arduino.h>
#include <Wire.h>

static PrintBuffer wireOutputBuffer;

size_t PrintBuffer::write(uint8_t v) {
  buffer.data[++buffer.len] = v;
  return 1;
}

void ExpanduinoI2C_::wireOnReceive(int _) {
  wireOutputBuffer.buffer.len = 0;

  //Received empty command  
  if (!Wire.available()) {
    return;
  }
  uint8_t cmd = Wire.read();

  uint8_t len = 0;  
  if (Wire.available()) {
    len = Wire.read();
  }
    
  ExpanduinoI2C.dispatch(cmd, Wire, wireOutputBuffer);

  while (Wire.available()) {
    Wire.read();
  }
}

void ExpanduinoI2C_::wireOnRequest() {
  Wire.write(wireOutputBuffer.buffer.data, wireOutputBuffer.buffer.len + 1);
}

void ExpanduinoI2C_::begin(int address, int interruptPin) {
  beginSubdevices();
  
  this->interruptPin = interruptPin;
  digitalWrite(interruptPin, false);
  pinMode(interruptPin, INPUT);
  
  Wire.begin(address);
  Wire.onReceive(ExpanduinoI2C_::wireOnReceive);
  Wire.onRequest(ExpanduinoI2C_::wireOnRequest);
}

void ExpanduinoI2C_::end() {
  Wire.end();
  pinMode(interruptPin, INPUT);
  
  endSubdevices();
}

bool ExpanduinoI2C_::readInterruptionData(Print& response) {
  pinMode(interruptPin, INPUT);
  bool ret = Expanduino::readInterruptionData(response);
  if (ret) {
    digitalWrite(interruptPin, false);
    pinMode(interruptPin, OUTPUT);
  }
  return ret;
}

bool ExpanduinoI2C_::requestInterruption(ExpanduinoSubdevice* dev) {
  bool ret = Expanduino::requestInterruption(dev);
  if (ret) {
    digitalWrite(interruptPin, false);
    pinMode(interruptPin, OUTPUT);
  }
  return ret;
}


