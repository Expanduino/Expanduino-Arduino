#include "hwserial.h"

ExpanduinoSubdeviceHardwareSerialArduino::ExpanduinoSubdeviceHardwareSerialArduino(Expanduino& container, const char* name, const char* shortName, HardwareSerial** serials, int numSerials) 
: ExpanduinoSubdeviceSerial(container, name, shortName),
  serials(serials),
  numSerials(numSerials)
{ }

void ExpanduinoSubdeviceHardwareSerialArduino::begin() {
  for (int i=0; i<numSerials; i++) {
    serials[i]->begin(9600);
  }
}

void ExpanduinoSubdeviceHardwareSerialArduino::end() {
  for (int i=0; i<numSerials; i++) {
    serials[i]->end();
  }
}

void ExpanduinoSubdeviceHardwareSerialArduino::pool() {
  bool interrupt = false;
  for (int i=0; i<numSerials; i++) {
    if (serials[i]->available()) {
      interrupt = true;
    }
  }
  if (interrupt) {
    noInterrupts();
    requestInterruption();
    interrupts();
  }
}

uint8_t ExpanduinoSubdeviceHardwareSerialArduino::getNumSerials() {
  return numSerials;
}

void ExpanduinoSubdeviceHardwareSerialArduino::write(uint8_t serialNum, Stream& data, uint8_t dataLen) {
  digitalWrite(4, HIGH);
  HardwareSerial* serial = this->serials[serialNum];
  uint8_t buffer[dataLen];
  int actualLen = data.readBytes(buffer, dataLen);
  serial->write(buffer, actualLen);
  digitalWrite(4, LOW);
  
}

void ExpanduinoSubdeviceHardwareSerialArduino::read(uint8_t serialNum, Print& data, uint8_t dataLen) {
  digitalWrite(3, HIGH);
  HardwareSerial* serial = this->serials[serialNum];
  uint8_t buffer[dataLen];
  digitalWrite(3, LOW);
  dataLen = min(dataLen, serial->available());
  int actualLen = serial->readBytes(buffer, dataLen);
  data.write(buffer, actualLen);
}

uint8_t ExpanduinoSubdeviceHardwareSerialArduino::available(uint8_t serialNum) {
  digitalWrite(2, HIGH);
  HardwareSerial* serial = this->serials[serialNum];
  return serial->available();
  digitalWrite(2, LOW);
}