#include "linux-input.h"

ExpanduinoSubdeviceLinuxInput::ExpanduinoSubdeviceLinuxInput(Expanduino& container, const char* name) 
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_LINUX_INPUT, "name", "linux-input")
{ }


void ExpanduinoSubdeviceLinuxInput::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    
    case EXPANDUINO_CMD_LINUX_INPUT_ID: {
      const LinuxInputId &id = getInputId();
      response.write((uint8_t)(id.vendor >> 8));
      response.write((uint8_t)(id.vendor >> 0));
      response.write((uint8_t)(id.product >> 8));
      response.write((uint8_t)(id.product >> 0));
      response.write((uint8_t)(id.version >> 8));
      response.write((uint8_t)(id.version >> 0));      
    }
    
    case EXPANDUINO_CMD_LINUX_INPUT_NUM_COMPONENTS: {
      response.write(getNumComponents());
      break;
    }
    
    case EXPANDUINO_CMD_LINUX_INPUT_GET_COMPONENT_TYPE: {
      if (request.available() >= 1) {
        uint8_t componentNum = request.read(); 
        if (componentNum < getNumComponents()) {
          const LinuxInputComponentType& type = getComponentType(componentNum);
          response.write((uint8_t)(type.type >> 8));
          response.write((uint8_t)(type.type >> 0));
          response.write((uint8_t)(type.code >> 8));
          response.write((uint8_t)(type.code >> 0));
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_LINUX_INPUT_GET_COMPONENT_VALUE: {
      if (request.available() >= 1) {
        uint8_t componentNum = request.read(); 
        if (componentNum < getNumComponents()) {
          int32_t value = getValue(componentNum);
          if (((int8_t)value) == value) {
            response.write((uint8_t)(value >>  0));
          } else if (((int16_t)value) == value) {
            response.write((uint8_t)(value >>  8));
            response.write((uint8_t)(value >>  0));
          } else {
            response.write((uint8_t)(value >> 24));
            response.write((uint8_t)(value >> 16));
            response.write((uint8_t)(value >>  8));
            response.write((uint8_t)(value >>  0));
          }
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_LINUX_INPUT_SET_COMPONENT_VALUE: {
      if (request.available() >= 2) {
        uint8_t componentNum = request.read();
        int32_t value = (int8_t)request.read();
        while (request.available() >= 1) {
          value = (value<<8) | (uint8_t)request.read();
        }
        setValue(componentNum, value);
      }
      break;
    }
  }
}

void ExpanduinoSubdeviceLinuxInput::reset() {
  int numElements = getNumComponents();
  for (int i=0; i<numElements; i++) {
    setValue(i, 0);
  }
}
