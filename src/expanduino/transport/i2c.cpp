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
  

  uint8_t len;  
  if (Wire.available()) {
    len = Wire.read();
  }
  if (len != Wire.available()) {
    //I2C message length is inconsistent;
    return;
  }
    
  ExpanduinoI2C.dispatch(cmd, Wire, wireOutputBuffer);

  while (Wire.available()) {
    Wire.read();
  }
}

char meh[] = {2, 65, 66, 0};
void ExpanduinoI2C_::wireOnRequest() {
//     Wire.write(meh, 4);
//     Serial.println("B");
    Wire.write(wireOutputBuffer.buffer.data, wireOutputBuffer.buffer.len + 1);
//   Serial.print("B");
//   for (int i=0; i<wireOutputBuffer.buffer.len + 1; i++) {
//     Serial.print(" ");
//     Serial.print(wireOutputBuffer.buffer.data[i]);
//   }
//   Serial.println();
}

void ExpanduinoI2C_::begin(int address, int interruptPin) {
  this->interruptPin = interruptPin;
  pinMode(interruptPin, OUTPUT);
  digitalWrite(interruptPin, false);
  
  this->address = address;
  Wire.begin(address);
  Wire.onReceive(ExpanduinoI2C_::wireOnReceive);
  Wire.onRequest(ExpanduinoI2C_::wireOnRequest);
}

void ExpanduinoI2C_::end() {
  Wire.end();
  pinMode(interruptPin, INPUT);
}


void ExpanduinoI2C_::requestInterrupt() {
  digitalWrite(interruptPin, true);
}


bool ExpanduinoI2C_::clearInterruptStatus() {
  bool ret = digitalRead(interruptPin);
  digitalWrite(interruptPin, false);
  return ret;
}

void ExpanduinoI2C_::getPhysicalLocation(Print& out) {
  out.print(F("expanduino-i2c@0x"));
  out.print(address, HEX);
}

ExpanduinoI2C_ ExpanduinoI2C;