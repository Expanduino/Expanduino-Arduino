#pragma once

#include "../expanduino.h"

enum ExpanduinoCommandLed {
  EXPANDUINO_CMD_LED_NUM_LEDS,
  EXPANDUINO_CMD_LED_NAME,
  EXPANDUINO_CMD_LED_GET_BRIGHTNESS,
  EXPANDUINO_CMD_LED_SET_BRIGHTNESS
};

class ExpanduinoSubdeviceLeds : public ExpanduinoSubdevice {
public:
  ExpanduinoSubdeviceLeds(Expanduino& container); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
  virtual void reset();
  virtual void getLedName(uint8_t ledNum, Print& name);
  virtual uint8_t getNumLeds() = 0;
  virtual uint8_t getBrightness(uint8_t ledNum) = 0;
  virtual void setBrightness(uint8_t ledNum, uint8_t brightness) = 0;
};
