#include "expanduino.h"

#include <Arduino.h> //FIXME

// class Expanduino ============================================================

Expanduino::Expanduino()
: metaSubdevice(*this)
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
    return;
  }
  
  uint8_t opcode = cmd & 0x0F;
  dev->dispatch(opcode, request, response);
}

void Expanduino::getVendorName(Print& out) {
  if (vendorName) {
    out.print(vendorName);
  } else {
    out.print(F("Generic"));
  }
}
void Expanduino::getProductName(Print& out) {
  if (productName) {
    out.print(productName);
  } else {
    out.print(F("Expanduino"));
  }
}
void Expanduino::getSerialNumber(Print& out) {
  if (serialNumber) {
    out.print(serialNumber);
  }
}

// class ExpanduinoSubdevice ======================================================

ExpanduinoSubdevice::ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType, const char* name, const char* physPrefix) 
: container(container),
  devType(devType),
  physPrefix(physPrefix),
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

void ExpanduinoSubdevice::getName(Print& out) {
  out.print(name);
}

void ExpanduinoSubdevice::getPhysicalLocation(Print& out) {
  container.getPhysicalLocation(out);
  out.print("/");
  out.print(physPrefix);
  out.print("@");
  out.print(devNum);
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
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_META, "Meta", "meta")
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
          response.write(dev->devType);
        }
      }     
      break;
    }
    
    EXPANDUINO_CMD_META_DEVICE_NAME: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          dev->getName(response);
        }
      }
      break;
    }
    
    EXPANDUINO_CMD_META_DEVICE_PHYS: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          dev->getPhysicalLocation(response);
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_META_VENDOR_NAME: {
      container.getVendorName(response);
      break;
    }
      
    case EXPANDUINO_CMD_META_PRODUCT_NAME: {
      container.getProductName(response);
      break;
    }
      
    case EXPANDUINO_CMD_META_SERIAL_NUMBER: {
      container.getSerialNumber(response);
      break;
    }
  
    case EXPANDUINO_CMD_META_PHYS: {
      container.getPhysicalLocation(response);
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
