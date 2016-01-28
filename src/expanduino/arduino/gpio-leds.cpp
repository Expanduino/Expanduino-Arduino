#include "gpio-leds.h"
#include <Arduino.h>

ExpanduinoSubdeviceGpioLedsArduino::ExpanduinoSubdeviceGpioLedsArduino(Expanduino& container, int numLeds, ArduinoLed* leds) 
: ExpanduinoSubdeviceLeds(container),
  numLeds(numLeds),
  leds(leds)
{
  for (int i=0; i<numLeds; i++) {
    pinMode(leds[i].pin, OUTPUT);
    setBrightness(i, 0);
  }
}

void ExpanduinoSubdeviceGpioLedsArduino::getLedName(uint8_t ledNum, Print& name) {
  ArduinoLed& led = leds[ledNum];
  if (led.name) {
    name.print(led.name);
  } else {
    ExpanduinoSubdeviceLeds::getLedName(ledNum, name);
  }
}

uint8_t ExpanduinoSubdeviceGpioLedsArduino::getNumLeds() {
  return numLeds;
}

uint8_t ExpanduinoSubdeviceGpioLedsArduino::getBrightness(uint8_t ledNum) {
  return leds[ledNum].brightness;
}

void ExpanduinoSubdeviceGpioLedsArduino::setBrightness(uint8_t ledNum, uint8_t brightness) {
  leds[ledNum].brightness = brightness;
  analogWrite(leds[ledNum].pin, brightness);
}
