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

char meh[] = {2, 65, 66, 0};
void ExpanduinoI2C_::wireOnRequest() {
  Wire.write(wireOutputBuffer.buffer.data, wireOutputBuffer.buffer.len + 1);

//   Serial.print(">>");
//   for (int i=0; i<wireOutputBuffer.buffer.len + 1; i++) {
//     Serial.print(" ");
//     Serial.print(wireOutputBuffer.buffer.data[i] / 16, HEX);
//     Serial.print(wireOutputBuffer.buffer.data[i] % 16, HEX);
//   }
//   Serial.println();
}

void ExpanduinoI2C_::begin(int address, int interruptPin) {
  this->interruptPin = interruptPin;
  pinMode(interruptPin, OUTPUT);
  digitalWrite(interruptPin, false);
  
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

ExpanduinoI2C_ ExpanduinoI2C;