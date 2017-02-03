#pragma once

#include "expanduino-subdevice.h"
#include "classes/meta.h"

#define LIBCALL_DEEP_SLEEP_SCHEDULER
#include <DeepSleepScheduler.h>

#define EXPANDUINO_MAX_RESPONSE_SIZE 128

class ExpanduinoInterruption {
public:
  ExpanduinoSubdevice* source;
  ExpanduinoInterruption* next;
};


class Expanduino {
protected:
  MetaExpanduinoSubdevice metaSubdevice;
  ExpanduinoInterruption* nextInterruption;
  
  friend class ExpanduinoSubdevice;
  friend class MetaExpanduinoSubdevice;
  
public:
  Expanduino();
  ~Expanduino();
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

  void main_loop();
  
  virtual bool readInterruptionData(Print& response);
  virtual bool requestInterruption(ExpanduinoSubdevice* dev);

  const char* vendorName;
  const char* productName;
  const char* serialNumber;
  const char* shortName;
};
