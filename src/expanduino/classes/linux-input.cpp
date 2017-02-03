#include "linux-input.h"
#include <Arduino.h>

ExpanduinoSubdeviceLinuxInput::ExpanduinoSubdeviceLinuxInput(Expanduino& container, const char* name, const char* shortName) 
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_LINUX_INPUT, name, shortName)
{ }


void ExpanduinoSubdeviceLinuxInput::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    
    case EXPANDUINO_CMD_LINUX_INPUT_ID: {
      const LinuxInputId &id = getLinuxInputId();
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
    
    case EXPANDUINO_CMD_LINUX_INPUT_COMPONENT_TYPE: {
      if (request.available() >= 1) {
        uint8_t componentNum = request.read(); 
        if (componentNum < getNumComponents()) {
          const LinuxInputComponentType type = getComponentType(componentNum);
          response.write((uint8_t)(type.type >> 8));
          response.write((uint8_t)(type.type >> 0));
          response.write((uint8_t)(type.code >> 8));
          response.write((uint8_t)(type.code >> 0));
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_LINUX_INPUT_COMPONENT_ABS_INFO: {
      if (request.available() >= 1) {
        uint8_t componentNum = request.read(); 
        if (componentNum < getNumComponents() && getComponentType(componentNum).type == EV_ABS) {
          const LinuxInputAbsoluteComponentConfig config = getAbsoluteComponentConfig(componentNum);
          response.write((uint8_t)(config.max >> 24));
          response.write((uint8_t)(config.max >> 16));
          response.write((uint8_t)(config.max >>  8));
          response.write((uint8_t)(config.max >>  0));
          
          response.write((uint8_t)(config.min >> 24));
          response.write((uint8_t)(config.min >> 16));
          response.write((uint8_t)(config.min >>  8));
          response.write((uint8_t)(config.min >>  0));
          
          response.write((uint8_t)(config.fuzz >> 24));
          response.write((uint8_t)(config.fuzz >> 16));
          response.write((uint8_t)(config.fuzz >>  8));
          response.write((uint8_t)(config.fuzz >>  0));

          response.write((uint8_t)(config.flat >> 24));
          response.write((uint8_t)(config.flat >> 16));
          response.write((uint8_t)(config.flat >>  8));
          response.write((uint8_t)(config.flat >>  0));
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_LINUX_INPUT_GET_COMPONENT_VALUE: {
      if (request.available() >= 1) {
        uint8_t componentNum = request.read(); 
        if (componentNum < getNumComponents()) {
          int32_t value = getValue(componentNum);
          response.write((uint8_t)(value >> 24));
          response.write((uint8_t)(value >> 16));
          response.write((uint8_t)(value >>  8));
          response.write((uint8_t)(value >>  0));
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

void ExpanduinoSubdeviceLinuxInput::readInterruptionData(Print& response) {
  LinuxInputStateChange values[LINUX_INPUT_MAX_INTERRUPT_EVENTS];
  uint8_t valuesLen = 0;
  this->getInterruptionReport(values, valuesLen);
  
  for (uint8_t i=0; i<valuesLen; i++) {
    response.write(values[i].component);
    response.write((uint8_t)(values[i].value >> 24));
    response.write((uint8_t)(values[i].value >> 16));
    response.write((uint8_t)(values[i].value >>  8));
    response.write((uint8_t)(values[i].value >>  0));
  }
}
