#include "gpio-lcd.h"
#include <Arduino.h>

ExpanduinoSubdeviceGpioLcdArduino::ExpanduinoSubdeviceGpioLcdArduino(Expanduino& container, const char* name, const char* shortName, AsyncLiquidCrystal& lcd, int pin_backlight) 
: ExpanduinoSubdeviceLcd(container, name, shortName),
  lcd(lcd),
  pin_backlight(pin_backlight)
{
}

void ExpanduinoSubdeviceGpioLcdArduino::begin() {
  lcd.begin(16,2);
  lcd.print("Veridis A400");
  scheduler.schedule(this);
    
  pinMode(pin_backlight, OUTPUT);
  analogWrite(pin_backlight, 0);
  brightness = 0;
}

void ExpanduinoSubdeviceGpioLcdArduino::end() {
  pinMode(pin_backlight, INPUT);
  scheduler.removeCallbacks(this);
}

void ExpanduinoSubdeviceGpioLcdArduino::reset() {
  lcd.begin(16,2);
}

void ExpanduinoSubdeviceGpioLcdArduino::run() {
  scheduler.removeCallbacks(this);
  long delay = lcd.processQueue();
  if (delay >= 0) {
    scheduler.scheduleDelayed(this, delay/1000);
  } else {
    scheduler.scheduleDelayed(this, 1);
  }
}

uint8_t ExpanduinoSubdeviceGpioLcdArduino::sendCommands(Stream& data, uint8_t dataLen) {
  uint8_t ret = 0;
  for (int i=0; i<dataLen; i++) {
    if(lcd.command(data.read())) {
      ret++;
    } else {
      break;
    }
  }
  return ret;
}

void ExpanduinoSubdeviceGpioLcdArduino::readText(Print& data, uint8_t dataLen) {
  for (int i=0; i<dataLen; i++) {
    // TODO: AsyncLiquidCrystal doesn't support reading back
    // data.write(lcd.read());
  }
}
  
uint8_t  ExpanduinoSubdeviceGpioLcdArduino::writeText(Stream& data, uint8_t dataLen) {
  uint8_t ret = 0;
  for (int i=0; i<dataLen; i++) {
    if(lcd.write(data.read())) {
      ret++;
    } else {
      break;
    }
  }
  return ret;
}

uint8_t ExpanduinoSubdeviceGpioLcdArduino::getBrightness() {
  return brightness;
}

void ExpanduinoSubdeviceGpioLcdArduino::setBrightness(uint8_t val) {
  analogWrite(pin_backlight, val);
  brightness = val;
}
