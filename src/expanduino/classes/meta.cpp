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
      container.reset();
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
