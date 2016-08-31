#include "gpio-lcd.h"
#include <Arduino.h>

ExpanduinoSubdeviceGpioLcdArduino::ExpanduinoSubdeviceGpioLcdArduino(Expanduino& container, const char* name, const char* shortName, LiquidCrystal& lcd, int pin_backlight) 
: ExpanduinoSubdeviceLcd(container, name, shortName),
  lcd(lcd),
  pin_backlight(pin_backlight)
{
}

void ExpanduinoSubdeviceGpioLcdArduino::begin() {
  lcd.begin(16,2);
  lcd.print("Veridis A400");
  pinMode(pin_backlight, OUTPUT);
  analogWrite(pin_backlight, 0);
  brightness = 0;
}

void ExpanduinoSubdeviceGpioLcdArduino::end() {
  pinMode(pin_backlight, INPUT);
}

void ExpanduinoSubdeviceGpioLcdArduino::reset() {
  lcd.begin(16,2);
}

void ExpanduinoSubdeviceGpioLcdArduino::sendCommands(Stream& data, uint8_t dataLen) {
  for (int i=0; i<dataLen; i++) {
    lcd.command(data.read());
  }
}

void ExpanduinoSubdeviceGpioLcdArduino::readText(Print& data, uint8_t dataLen) {
  for (int i=0; i<dataLen; i++) {
    // TODO: LiquidCrystal doesn't support reading back
    // data.write(lcd.read());
  }
}
  
void ExpanduinoSubdeviceGpioLcdArduino::writeText(Stream& data, uint8_t dataLen) {
  for (int i=0; i<dataLen; i++) {
    lcd.write(data.read());
  }
}

uint8_t ExpanduinoSubdeviceGpioLcdArduino::getBrightness() {
  return brightness;
}

void ExpanduinoSubdeviceGpioLcdArduino::setBrightness(uint8_t val) {
  analogWrite(pin_backlight, val);
  brightness = val;
}
