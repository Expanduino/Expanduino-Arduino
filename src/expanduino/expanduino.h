#pragma once

#include "expanduino-subdevice.h"
#include "classes/meta.h"

#define EXPANDUINO_MAX_RESPONSE_SIZE 128

class ExpanduinoInterruption {
public:
  ExpanduinoSubdevice* source;
  ExpanduinoInterruption* next;
};


class Expanduino {
  MetaExpanduinoSubdevice metaSubdevice;
  const char* vendorName;
  const char* productName;
  const char* serialNumber;
  const char* shortName;
  ExpanduinoInterruption* nextInterruption;
  
  friend class ExpanduinoSubdevice;
  friend class MetaExpanduinoSubdevice;
  
public:
  Expanduino();
  uint8_t getNumSubdevices();
  ExpanduinoSubdevice* getDevice(uint8_t devNumber);
  void dispatch(uint8_t cmd, Stream& request, Print& response);
  void getVendorName(Print& out);
  void getProductName(Print& out);
  void getSerialNumber(Print& out);
  void getShortName(Print& out);
  void beginSubdevices();
  void endSubdevices();
  void reset();
  
  virtual bool readInterruptionData(Print& response);
  virtual bool requestInterruption(ExpanduinoSubdevice* dev);

};
