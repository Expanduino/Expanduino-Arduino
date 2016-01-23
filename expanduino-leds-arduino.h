#pragma once

#include "expanduino-leds.h"

typedef struct {
  int pin;
  const char* name;
  uint8_t brightness;
} ArduinoLed;

class ExpanduinoSubdeviceLedsArduino : public ExpanduinoSubdeviceLeds {
  int numLeds;
  ArduinoLed* leds;
public:
  ExpanduinoSubdeviceLedsArduino(Expanduino& container, int numLeds, ArduinoLed* leds);
  virtual uint8_t getNumLeds();
  virtual uint8_t getBrightness(uint8_t ledNum);
  virtual void setBrightness(uint8_t ledNum, uint8_t brightness);
  virtual void getLedName(uint8_t ledNum, Print& name);
};
