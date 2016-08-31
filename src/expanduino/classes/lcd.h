#pragma once

#include "../expanduino.h"

enum ExpanduinoCommandLcd {
  EXPANDUINO_CMD_LCD_CMD             = 0,
  EXPANDUINO_CMD_LCD_READ_TEXT       = 1,
  EXPANDUINO_CMD_LCD_WRITE_TEXT      = 2,
  EXPANDUINO_CMD_LCD_GET_BRIGHTNESS  = 3,
  EXPANDUINO_CMD_LCD_SET_BRIGHTNESS  = 4
};

class ExpanduinoSubdeviceLcd : public ExpanduinoSubdevice {
public:
  ExpanduinoSubdeviceLcd(Expanduino& container, const char* name="LCD", const char* shortName="lcd"); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
  
  virtual void sendCommands(Stream& data, uint8_t dataLen) = 0;
  virtual void readText(Print& data, uint8_t dataLen) = 0;
  virtual void writeText(Stream& data, uint8_t dataLen) = 0;
  virtual uint8_t getBrightness() = 0;
  virtual void setBrightness(uint8_t val) = 0;
};

