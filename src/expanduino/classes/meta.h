#pragma once

#include "../expanduino-subdevice.h"

enum ExpanduinoCommandMeta {
  EXPANDUINO_CMD_META_VENDOR_NAME              = 0,
  EXPANDUINO_CMD_META_PRODUCT_NAME             = 1,
  EXPANDUINO_CMD_META_SHORT_NAME               = 2,
  EXPANDUINO_CMD_META_SERIAL_NUMBER            = 3,
  EXPANDUINO_CMD_META_RESET                    = 4,
  EXPANDUINO_CMD_META_GET_INTERRUPTION         = 5,
  EXPANDUINO_CMD_META_GET_INTERRUPTION_ENABLED = 6,
  EXPANDUINO_CMD_META_SET_INTERRUPTION_ENABLED = 7,
  EXPANDUINO_CMD_META_NUM_SUBDEVICES           = 8,
  EXPANDUINO_CMD_META_SUBDEVICE_TYPE           = 9,
  EXPANDUINO_CMD_META_SUBDEVICE_NAME           = 10,
  EXPANDUINO_CMD_META_SUBDEVICE_SHORT_NAME     = 11
};

class MetaExpanduinoSubdevice : public ExpanduinoSubdevice {
public:
  MetaExpanduinoSubdevice(Expanduino& container); 
  virtual void dispatch(uint8_t opcode, Stream& request, Print& response);
};
