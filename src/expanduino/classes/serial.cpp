#include "serial.h"

#define DEFAULT_READ_SIZE 25

ExpanduinoSubdeviceSerial::ExpanduinoSubdeviceSerial(Expanduino& container, const char* name, const char* shortName) 
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_SERIAL, name, shortName)
{ }


void ExpanduinoSubdeviceSerial::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    case EXPANDUINO_CMD_SERIAL_NUM_SERIALS: {
      response.write(getNumSerials());
      break;
    }
    case EXPANDUINO_CMD_SERIAL_NAME: {
      if (request.available()) {
        uint8_t serialNum = request.read();
        if (serialNum < getNumSerials()) {
          getSerialName(serialNum, response);
        }
      }
      break;
    }    
    case EXPANDUINO_CMD_SERIAL_WRITE: {
      if (request.available()) {
        uint8_t serialNum = request.read();
        if (serialNum < getNumSerials()) {
          this->write(serialNum, request, request.available());
        }
      }
      break;
    }
    case EXPANDUINO_CMD_SERIAL_READ: {
      if (request.available()) {
        uint8_t serialNum = request.read();
        uint8_t readLen = request.read();
        if (serialNum < getNumSerials()) {
          this->write(serialNum, request, readLen);
        }
      }
      break;
    }
    case EXPANDUINO_CMD_SERIAL_AVAILABLE: {
      if (request.available()) {
        uint8_t serialNum = request.read();
        if (serialNum < getNumSerials()) {
          response.write(this->available(serialNum));
        }
      }
      break;
    }
  }
}

void ExpanduinoSubdeviceSerial::readInterruptionData(Print& response) {
  uint8_t numSerials = this->getNumSerials();
  uint8_t serialNum = nextInterruptFrom;
  bool handled = false;
  
  for (uint8_t i=0; i<numSerials; i++) {
    uint8_t available = this->available(serialNum);
    
    if (available) {
      if (!handled) {
        response.write(serialNum);
        
        this->read(serialNum, response, DEFAULT_READ_SIZE);
        handled = true;
        nextInterruptFrom = serialNum+1;
        if (nextInterruptFrom >= numSerials) {
          nextInterruptFrom = 0;
        }
        available = this->available(serialNum);
      }
      if (available) {
        this->requestInterruption();
        break;
      }
    }
    
    serialNum++;
    if (serialNum >= numSerials) {
      serialNum = 0;
    }
  }
}

void ExpanduinoSubdeviceSerial::getSerialName(uint8_t serialNum, Print& name) {
  this->getShortName(name);
  name.print("-");
  name.print(serialNum);
}