#pragma once
#include <inttypes.h>
#include <Stream.h>

enum ExpanduinoSubdeviceType {
  EXPANDUINO_DEVICE_TYPE_MISSING      = 0,
  EXPANDUINO_DEVICE_TYPE_META         = 1,
  EXPANDUINO_DEVICE_TYPE_LEDS         = 2,
  EXPANDUINO_DEVICE_TYPE_GPIO         = 3,
  EXPANDUINO_DEVICE_TYPE_LINUX_INPUT  = 4,
  EXPANDUINO_DEVICE_TYPE_HID          = 5,
  EXPANDUINO_DEVICE_TYPE_SERIAL       = 6,
  EXPANDUINO_DEVICE_TYPE_MISC         = 7,
  EXPANDUINO_DEVICE_TYPE_I2C          = 8,
  EXPANDUINO_DEVICE_TYPE_SPI          = 9
};

class Expanduino;
class ExpanduinoSubdevice;
class MetaExpanduinoSubdevice;

class ExpanduinoSubdevice {
  Expanduino& container;
  ExpanduinoSubdeviceType devType;
  bool isInterrupted;
  ExpanduinoSubdevice* next;
  uint8_t devNum;
  const char* name;
  const char* shortName;
  
  friend class Expanduino;
  friend class MetaExpanduinoSubdevice;
  
public:
  ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType, const char* name, const char* shortName);
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response) = 0;
  virtual void reset();
  void requestInterrupt();
  bool clearInterruptStatus();
  void getName(Print& out);
  void getShortName(Print& out);
};
