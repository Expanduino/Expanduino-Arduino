#pragma once

#include "../classes/leds.h"

typedef struct {
  int pin;
  const char* name;
  uint8_t brightness;
} ArduinoLed;

class ExpanduinoSubdeviceGpioLedsArduino : public ExpanduinoSubdeviceLeds {
  ArduinoLed* leds;
  int numLeds;
public:
  ExpanduinoSubdeviceGpioLedsArduino(Expanduino& container, const char* name, const char* shortName, ArduinoLed* leds, int numLeds);
  virtual void begin();
  virtual void end();
  virtual uint8_t getNumLeds();
  virtual uint8_t getBrightness(uint8_t ledNum);
  virtual void setBrightness(uint8_t ledNum, uint8_t brightness);
  virtual void getLedName(uint8_t ledNum, Print& name);
};
