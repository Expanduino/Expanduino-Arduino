#pragma once

#include <inttypes.h>
#include <Stream.h>

#define EXPANDUINO_MAX_RESPONSE_SIZE 64

enum ExpanduinoSubdeviceType {
  EXPANDUINO_DEVICE_TYPE_MISSING  = 0,
  EXPANDUINO_DEVICE_TYPE_META     = 1,
  EXPANDUINO_DEVICE_TYPE_LED      = 2,
  EXPANDUINO_DEVICE_TYPE_GPIO     = 3,
  EXPANDUINO_DEVICE_TYPE_INPUT    = 4,
  EXPANDUINO_DEVICE_TYPE_HID      = 5,
  EXPANDUINO_DEVICE_TYPE_SERIAL   = 6,
  EXPANDUINO_DEVICE_TYPE_MISC     = 7,
  EXPANDUINO_DEVICE_TYPE_I2C      = 8,
  EXPANDUINO_DEVICE_TYPE_SPI      = 9
};

enum ExpanduinoCommandMeta {
  EXPANDUINO_CMD_META_RESET              = 0,
  EXPANDUINO_CMD_META_NUM_SUBDEVICES     = 1,
  EXPANDUINO_CMD_META_GET_INTERRUPTS     = 2,
  EXPANDUINO_CMD_META_DEVICE_TYPE        = 3,
  EXPANDUINO_CMD_META_VENDOR_NAME        = 4,
  EXPANDUINO_CMD_META_PRODUCT_NAME       = 5,
  EXPANDUINO_CMD_META_SERIAL_NUMBER      = 6
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
  
  friend class Expanduino;
  friend class MetaExpanduinoSubdevice;
  
public:
  ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType);
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response) = 0;
  virtual void reset() = 0;
  void requestInterrupt();
  bool clearInterruptStatus();
};

class MetaExpanduinoSubdevice : public ExpanduinoSubdevice {
public:
  MetaExpanduinoSubdevice(Expanduino& container); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
  virtual void reset();
};

class Expanduino {
  MetaExpanduinoSubdevice metaSubdevice;
  const char* vendorName;
  const char* productName;
  const char* serialNumber;
  
  friend class ExpanduinoSubdevice;
  friend class MetaExpanduinoSubdevice;
  
public:
  Expanduino();
  uint8_t getNumSubdevices();
  ExpanduinoSubdevice* getDevice(uint8_t devNumber);
  void dispatch(uint8_t cmd, Stream& request, Print& response);
  virtual void requestInterrupt() = 0;
  virtual bool clearInterruptStatus() = 0;
};