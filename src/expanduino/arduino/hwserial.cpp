#include "hwserial.h"

ExpanduinoSubdeviceHardwareSerialArduino::ExpanduinoSubdeviceHardwareSerialArduino(Expanduino& container, int numSerials, HardwareSerial** serials) 
: ExpanduinoSubdeviceSerial(container),
  numSerials(numSerials),
  serials(serials)
{ }


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