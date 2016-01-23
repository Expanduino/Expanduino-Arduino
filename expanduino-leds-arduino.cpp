#include "expanduino-leds-arduino.h"
#include <Arduino.h>

ExpanduinoSubdeviceLedsArduino::ExpanduinoSubdeviceLedsArduino(Expanduino& container, int numLeds, ArduinoLed* leds) 
: ExpanduinoSubdeviceLeds(container),
  leds(leds),
  numLeds(numLeds)
{
  for (int i=0; i<numLeds; i++) {
    setBrightness(i, 0);
  }
}

void ExpanduinoSubdeviceLedsArduino::getLedName(uint8_t ledNum, Print& name) {
  ArduinoLed& led = leds[ledNum];
  if (led.name) {
    name.print(led.name);
  } else {
    ExpanduinoSubdeviceLeds::getLedName(ledNum, name);
  }
}

uint8_t ExpanduinoSubdeviceLedsArduino::getNumLeds() {
  return numLeds;
}

uint8_t ExpanduinoSubdeviceLedsArduino::getBrightness(uint8_t ledNum) {
  return leds[ledNum].brightness;
}

void ExpanduinoSubdeviceLedsArduino::setBrightness(uint8_t ledNum, uint8_t brightness) {
  leds[ledNum].brightness = brightness;
  analogWrite(leds[ledNum].pin, brightness);
}
