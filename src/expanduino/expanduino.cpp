#include <DeepSleepScheduler.h>

#include "expanduino.h"

Expanduino::Expanduino()
: metaSubdevice(*this),
  nextInterruption(nullptr)
{
  scheduler.acquireNoDeepSleepLock();
}

Expanduino::~Expanduino() {
  scheduler.releaseNoDeepSleepLock();
}


uint8_t Expanduino::getNumSubdevices() {
  uint8_t numSubdevices = 0;
  ExpanduinoSubdevice* dev = &this->metaSubdevice;
  while (dev) {
    numSubdevices++;
    dev = dev->next;
  }
  return numSubdevices;
}
 
//Gets the n-th subdevice
ExpanduinoSubdevice* Expanduino::getDevice(uint8_t devNum)  {
  ExpanduinoSubdevice* dev = &this->metaSubdevice;
  for (int i=0; i<devNum && dev; i++) {
    dev = dev->next;
  }
  return dev;
}

//Dispatches a request to the correct device
void Expanduino::dispatch(uint8_t cmd, Stream& request, Print& response) {
  uint8_t devNum = (cmd >> 4) & 0x0F;
  ExpanduinoSubdevice* dev = getDevice(devNum);
  if (!dev) { //Invalid devNum
    return;
  }
  
  uint8_t opcode = cmd & 0x0F;
  dev->dispatch(opcode, request, response);
}

void Expanduino::getVendorName(Print& out) {
  out.print(vendorName ? vendorName : "Generic");
}
void Expanduino::getProductName(Print& out) {
  out.print(productName ? productName : "Expanduino");
}
void Expanduino::getSerialNumber(Print& out) {
  out.print(serialNumber ? serialNumber : "N/A");
}
void Expanduino::getShortName(Print& out) {
  out.print(shortName ? shortName : "expanduino");
}


void Expanduino::beginSubdevices() {
  ExpanduinoSubdevice* dev = &this->metaSubdevice;
  while (dev) {
    dev->begin();
    dev = dev->next;
  }
}

void Expanduino::endSubdevices() {
  ExpanduinoSubdevice* dev = &this->metaSubdevice;
  while (dev) {
    dev->end();
    dev = dev->next;
  }
}

void Expanduino::reset() {
  ExpanduinoSubdevice* dev = &this->metaSubdevice;
  while (dev) {
    dev->setInterruptionEnabled(false);
    dev->reset();
    dev = dev->next;
  }
}

bool Expanduino::requestInterruption(ExpanduinoSubdevice* dev) {
  if (!dev->interruptionEnabled) {
    return false;
  } else if (dev->interrupted) {
    return false;
  }
  
  dev->interrupted = true;
  
  ExpanduinoInterruption* interruption = (ExpanduinoInterruption*)malloc(sizeof(ExpanduinoInterruption));
  interruption->source = dev;
  interruption->next = nullptr;
  
  ExpanduinoInterruption** ptrNextInterruption = &this->nextInterruption;
  while (*ptrNextInterruption) {
    ptrNextInterruption = &((*ptrNextInterruption)->next);
  }
  *ptrNextInterruption = interruption;
  
  return true;
}

bool Expanduino::readInterruptionData(Print& response) {
  ExpanduinoInterruption* interruption = this->nextInterruption;
  if (!interruption) {
    return false;
  }
  
  ExpanduinoSubdevice* dev = interruption->source;
  dev->interrupted = false;
  response.write(dev->devNum);
  dev->readInterruptionData(response);
  
  this->nextInterruption = interruption->next;
  free(interruption);
  
  return this->nextInterruption;
}

void Expanduino::main_loop() {
  scheduler.execute();
}
