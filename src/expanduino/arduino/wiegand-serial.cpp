#include "wiegand-serial.h"
#include <YetAnotherPcInt.h>

#define RUN_DELAY 100

static void receivedData(uint8_t* data, uint8_t bits, LoopbackStream* out) {
  uint8_t bytes = (bits+7)/8;
  
  //If we don't have enough space in the buffer, just ignore it.
  if (out->availableForWrite() < 2*bytes + 2) {
    return;
  }
  for (int i=0; i<bytes; i++) {
    out->print((int)(data[i] >> 4), 16);
    out->print((int)(data[i] & 0xF), 16);
  }
  out->println();
}
void pin0StateChanged(Wiegand* wiegand, bool pinstate) {
  wiegand->setPin0State(pinstate);
}
void pin1StateChanged(Wiegand* wiegand, bool pinstate) {
  wiegand->setPin1State(pinstate);
}




ExpanduinoSubdeviceWiegandSerialArduino::ExpanduinoSubdeviceWiegandSerialArduino(Expanduino& container, const char* name, const char* shortName, ArduinoWiegandComponent* wiegands, int numWiegands) 
: ExpanduinoSubdeviceSerial(container, name, shortName),
  wiegands(wiegands),
  numWiegands(numWiegands)
{ }

void ExpanduinoSubdeviceWiegandSerialArduino::begin() {
  for (int i=0; i<numWiegands; i++) {
    wiegands[i].buffer.flush();
    wiegands[i].wiegand.onReceive(receivedData, &wiegands[i].buffer);
    wiegands[i].wiegand.begin(Wiegand::LENGTH_ANY, true);
    pinMode(wiegands[i].pin0, INPUT_PULLUP);
    pinMode(wiegands[i].pin1, INPUT_PULLUP);
    PcInt::attachInterrupt(wiegands[i].pin0, pin0StateChanged, &wiegands[i].wiegand, CHANGE, true);
    PcInt::attachInterrupt(wiegands[i].pin1, pin1StateChanged, &wiegands[i].wiegand, CHANGE, true);
  }
  scheduler.schedule(this);
}

void ExpanduinoSubdeviceWiegandSerialArduino::end() {
  scheduler.removeCallbacks(this);
  for (int i=0; i<numWiegands; i++) {
    wiegands[i].wiegand.end();
    PcInt::detachInterrupt(wiegands[i].pin0);
    PcInt::detachInterrupt(wiegands[i].pin1);
    pinMode(wiegands[i].pin0, INPUT);
    pinMode(wiegands[i].pin1, INPUT);
  }
}

void ExpanduinoSubdeviceWiegandSerialArduino::run() {
  bool interrupt = false;
  for (int i=0; i<numWiegands; i++) {
    ArduinoWiegandComponent& src = wiegands[i];
    src.wiegand.flush();
    if (src.buffer.available()) {
      interrupt = true;
    }
  }
  if (interrupt) {
    noInterrupts();
    requestInterruption();
    interrupts();
  }
  scheduler.scheduleDelayed(this, RUN_DELAY);
}

uint8_t ExpanduinoSubdeviceWiegandSerialArduino::getNumSerials() {
  return numWiegands;
}

void ExpanduinoSubdeviceWiegandSerialArduino::write(uint8_t serialNum, Stream& data, uint8_t dataLen) {
}

void ExpanduinoSubdeviceWiegandSerialArduino::read(uint8_t serialNum, Print& data, uint8_t dataLen) {
  LoopbackStream& src = this->wiegands[serialNum].buffer;
  uint8_t buffer[dataLen];
  dataLen = min(dataLen, src.available());
  int actualLen = src.readBytes(buffer, dataLen);
  data.write(buffer, actualLen);
}

uint8_t ExpanduinoSubdeviceWiegandSerialArduino::available(uint8_t serialNum) {
  LoopbackStream& src = this->wiegands[serialNum].buffer;
  return src.available();
}
