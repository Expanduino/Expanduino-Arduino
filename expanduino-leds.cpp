#include "expanduino-leds.h"

ExpanduinoSubdeviceLeds::ExpanduinoSubdeviceLeds(Expanduino& container) 
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_LED)
{ }


void ExpanduinoSubdeviceLeds::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    
    case EXPANDUINO_CMD_LED_NUM_LEDS: {
      response.write(getNumLeds());
      break;
    }
    
    case EXPANDUINO_CMD_LED_NAME: {
      if (request.available() >= 1) {
        int ledNum = request.read(); 
        if (ledNum < getNumLeds()) {
          getLedName(ledNum, response);
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_LED_GET_BRIGHTNESS: {
      if (request.available() >= 1) {
        int ledNum = request.read(); 
        if (ledNum < getNumLeds()) {
          response.write(getBrightness(ledNum));
        }
      }
      break;
    }
    
    case EXPANDUINO_CMD_LED_SET_BRIGHTNESS: {
      if (request.available() >= 2) {
        int ledNum = request.read(); 
        int brightness = request.read();
        if (ledNum < getNumLeds()) {
          setBrightness(ledNum, brightness);
        }
      }
      break;
    }
  }
}

void ExpanduinoSubdeviceLeds::reset() {
  int numLeds = getNumLeds();
  for (int i=0; i<numLeds; i++) {
    setBrightness(i, 0);
  }
}

void ExpanduinoSubdeviceLeds::getLedName(uint8_t ledNum, Print& name) {
  name.print("expanduino:led:");
  name.print(ledNum);
}