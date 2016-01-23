#include "expanduino.h"

#include <Arduino.h> //FIXME

// class Expanduino ============================================================

Expanduino::Expanduino()
: metaSubdevice(*this),
  vendorName("Generic"),
  productName("Expanduino"),
  serialNumber("0000")
{ }


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
    Serial.print("Called Invalid subdevice #");
    Serial.println(devNum, HEX);
    return;
  }
  
  uint8_t opcode = cmd & 0x0F;
  dev->dispatch(opcode, request, response);
}

// class ExpanduinoSubdevice ======================================================

ExpanduinoSubdevice::ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType) 
: container(container),
  devType(devType),
  isInterrupted(false),
  next(nullptr),
  devNum(0)
{
  //Adiciona este componente na lista ligada
  ExpanduinoSubdevice* lastSubdevice = &container.metaSubdevice;
  if (this != lastSubdevice) { //Não executa no metaSubdevice
    while (lastSubdevice->next) {
      lastSubdevice = lastSubdevice->next;
    }
    lastSubdevice->next = this;
    this->devNum = lastSubdevice->devNum + 1;
  }
}

void ExpanduinoSubdevice::requestInterrupt() {
  if (!this->isInterrupted) {
    this->isInterrupted = true;
    container.requestInterrupt();
  }
}

bool ExpanduinoSubdevice::clearInterruptStatus() {
  bool ret = this->isInterrupted;
  this->isInterrupted = false;
  return ret;
}

// class MetaExpanduinoSubdevice ===============================================

MetaExpanduinoSubdevice::MetaExpanduinoSubdevice(Expanduino& container) 
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_META)
{ }


void MetaExpanduinoSubdevice::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    
    case EXPANDUINO_CMD_META_RESET: {
      this->reset();
      break;
    }
    
    case EXPANDUINO_CMD_META_GET_INTERRUPTS: { //TODO: Test
      uint8_t pos = 0;
      uint8_t bits = 0;
      ExpanduinoSubdevice* dev = this;
      while (dev) {
        if (pos == 8) {
          response.write(bits);
          bits = 0;
          pos = 0;
        }
        //The meta-device's interrupt flag is replaced with the global interrupt flag
        bool isInterrupted = dev==this ? container.clearInterruptStatus() : dev->clearInterruptStatus();
        if (isInterrupted) {
          bits |= 1<<pos;
        }
        pos++;
        dev = dev->next;
      }
      break;
    }
    
    case EXPANDUINO_CMD_META_NUM_SUBDEVICES: {
      response.write(container.getNumSubdevices());
      break;
    }
      
    case EXPANDUINO_CMD_META_DEVICE_TYPE: {
      uint8_t devType = EXPANDUINO_DEVICE_TYPE_MISSING;
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          devType = dev->devType;
        }
      }
      response.write(devType);
      break;
    }
      
    case EXPANDUINO_CMD_META_VENDOR_NAME: {
      response.write(container.vendorName);
      break;
    }
      
    case EXPANDUINO_CMD_META_PRODUCT_NAME: {
      response.write(container.productName);
      break;
    }
      
    case EXPANDUINO_CMD_META_SERIAL_NUMBER: {
      response.write(container.serialNumber);
      break;
    }
  }
}

void MetaExpanduinoSubdevice::reset() {
  ExpanduinoSubdevice* other = this->next;
  while (other) {
    other->reset();
    other = other->next;
  }
}
