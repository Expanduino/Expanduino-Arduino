#pragma once

#include "../classes/lcd.h"
#include <AsyncLiquidCrystal.h>

class ExpanduinoSubdeviceGpioLcdArduino : public ExpanduinoSubdeviceLcd, public Runnable {
  AsyncLiquidCrystal& lcd;
  uint8_t brightness;
  int pin_backlight;
public:
  ExpanduinoSubdeviceGpioLcdArduino(Expanduino& container, const char* name, const char* shortName, AsyncLiquidCrystal& lcd, int pin_backlight);
  virtual void begin();
  virtual void end();
  
  virtual void run();
  virtual void reset();
  virtual uint8_t sendCommands(Stream& data, uint8_t dataLen);
  virtual void readText(Print& data, uint8_t dataLen);
  virtual uint8_t writeText(Stream& data, uint8_t dataLen);
  virtual uint8_t getBrightness();
  virtual void setBrightness(uint8_t val);
};
