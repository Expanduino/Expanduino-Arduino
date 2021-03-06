#include "lcd.h"

ExpanduinoSubdeviceLcd::ExpanduinoSubdeviceLcd(Expanduino& container, const char* name, const char* shortName) 
: ExpanduinoSubdevice(container, EXPANDUINO_DEVICE_TYPE_LCD, name, shortName)
{ }

void ExpanduinoSubdeviceLcd::dispatch(uint8_t opcode, Stream& request, Print& response) {
  switch (opcode) {
    case EXPANDUINO_CMD_LCD_CMD: {
      uint8_t writeLen = 0;
      if (request.available()) {
        writeLen = this->sendCommands(request, request.available());
      }
      response.write(writeLen);
      break;
    }    
    case EXPANDUINO_CMD_LCD_READ_TEXT: {
      if (request.available()) {
        uint8_t readLen = request.read();
        this->readText(response, readLen);
      }
      break;
    }
    case EXPANDUINO_CMD_LCD_WRITE_TEXT: {
      uint8_t writeLen = 0;
      if (request.available()) {
        writeLen = this->writeText(request, request.available());
      }
      response.write(writeLen);
      break;
    }
    case EXPANDUINO_CMD_LCD_GET_BRIGHTNESS: {
      response.write(this->getBrightness());
      break;
    }
    case EXPANDUINO_CMD_LCD_SET_BRIGHTNESS: {
      if (request.available()) {
        this->setBrightness(request.read());
      }
      break;
    }
  }
}
