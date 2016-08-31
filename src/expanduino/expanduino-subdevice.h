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
  EXPANDUINO_DEVICE_TYPE_SPI          = 9,
  EXPANDUINO_DEVICE_TYPE_LCD          = 10
};

class Expanduino;
class ExpanduinoSubdevice;
class MetaExpanduinoSubdevice;

class ExpanduinoSubdevice {
  Expanduino& container;
  ExpanduinoSubdeviceType devType;
  bool interruptionEnabled;
  bool interrupted;
  ExpanduinoSubdevice* next;
  uint8_t devNum;
  const char* name;
  const char* shortName;
  
  friend class Expanduino;
  friend class MetaExpanduinoSubdevice;
  
public:
  ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType, const char* name, const char* shortName);
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response) = 0;
  virtual void begin();
  virtual void end();
  virtual void reset();
  virtual void pool();
  
  //Human-Friendly name
  void getName(Print& out);
  
  //Machine-friendly name: Short, no spaces, etc
  void getShortName(Print& out);
   
  // Called from the hosts to enable/disable interrupts from this subdevice
  void setInterruptionEnabled(bool enabled);
  virtual void startedInterruption();
  virtual void stoppedInterruption();
  
  // Notify the controller about an interruption. 
  // If the result is false, the host isn't expecting interruptions (interrupts_enabled == false) and the event should be discarded.
  // Otherwise, readInterruptionData() will be called ASAP
  bool requestInterruption();

  // Discard any pending interruptions -- override if needed
  virtual void clearInterruption();  
 
  // Returns interruption data -- Override if needed
  // If there is more data to read, requestInterrupt() may be called from it.
  virtual void readInterruptionData(Print& response);
};
