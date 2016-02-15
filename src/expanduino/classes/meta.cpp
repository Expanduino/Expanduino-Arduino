#include "expanduino.h"

MetaExpanduinoSubdevice::MetaExpanduinoSubdevice(Expanduino& container)
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_META, "Meta", "meta")
{ }


void MetaExpanduinoSubdevice::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    
    case EXPANDUINO_CMD_META_VENDOR_NAME: {
      container.getVendorName(response);
      break;
    }
      
    case EXPANDUINO_CMD_META_PRODUCT_NAME: {
      container.getProductName(response);
      break;
    }
     
    case EXPANDUINO_CMD_META_SHORT_NAME: {
      container.getShortName(response);
      break;
    }
    
    case EXPANDUINO_CMD_META_SERIAL_NUMBER: {
      container.getSerialNumber(response);
      break;
    }
    
    case EXPANDUINO_CMD_META_RESET: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          dev->setInterruptionEnabled(false);
          dev->reset();
        }
      } else {
        container.reset();
      }
      break;
    }
        
    case EXPANDUINO_CMD_META_GET_INTERRUPTION: {
      container.readInterruptionData(response);
      break;
    }
    
    case EXPANDUINO_CMD_META_GET_INTERRUPTION_ENABLED: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          response.write(dev->interruptionEnabled);
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_META_SET_INTERRUPTION_ENABLED: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev && request.available()) {
          dev->setInterruptionEnabled(request.read());
        }
      }
      break;
    }
        
    case EXPANDUINO_CMD_META_NUM_SUBDEVICES: {
      response.write(container.getNumSubdevices());
      break;
    }   
      
    case EXPANDUINO_CMD_META_SUBDEVICE_TYPE: {
      uint8_t devType = EXPANDUINO_DEVICE_TYPE_MISSING;
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          response.write(dev->devType);
        }
      }     
      break;
    }
    
    case EXPANDUINO_CMD_META_SUBDEVICE_NAME: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          dev->getName(response);
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_META_SUBDEVICE_SHORT_NAME: {
      if (request.available()) {
        ExpanduinoSubdevice* dev = container.getDevice(request.read());
        if (dev) {
          dev->getShortName(response);
        }
      }
      break;
    }
  }
}
