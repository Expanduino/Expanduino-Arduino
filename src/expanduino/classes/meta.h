#pragma once

#include "../expanduino-subdevice.h"

enum ExpanduinoCommandMeta {
  EXPANDUINO_CMD_META_VENDOR_NAME          = 0,
  EXPANDUINO_CMD_META_PRODUCT_NAME         = 1,
  EXPANDUINO_CMD_META_SHORT_NAME           = 2,
  EXPANDUINO_CMD_META_SERIAL_NUMBER        = 3,
  EXPANDUINO_CMD_META_RESET                = 4,
  EXPANDUINO_CMD_META_GET_INTERRUPTS       = 5,
  EXPANDUINO_CMD_META_NUM_SUBDEVICES       = 6,
  EXPANDUINO_CMD_META_SUBDEVICE_TYPE       = 7,
  EXPANDUINO_CMD_META_SUBDEVICE_NAME       = 8,
  EXPANDUINO_CMD_META_SUBDEVICE_SHORT_NAME = 9
};

class MetaExpanduinoSubdevice : public ExpanduinoSubdevice {
public:
  MetaExpanduinoSubdevice(Expanduino& container); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
};
