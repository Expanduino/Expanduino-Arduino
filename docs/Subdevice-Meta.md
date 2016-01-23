# Meta subdevice

The meta subdevice is present in all Expanduinos and receives the subdevice id #0.

It is responsible to provide to the host metainformation about the device and all of it's subdevices:
- Vendor name, product Name and serial number
- How many subdevices
- What are the types of each subdevice (Led, LCD, Serial Port, etc)
- Which subdevices requite interrupt handling


# Operations

## `EXPANDUINO_CMD_META_RESET`
Resets all subdevices to their default states.
- Parameters: None
- Response: None


## `EXPANDUINO_CMD_META_GET_INTERRUPTS`
Returns a bitset of all devices requiring atention.

After this method is called, the flags of each subdevice are cleared immediately.
- Parameters: None
- Response: 
  - `flags`: 2 bytes, where the `(i%8)`-th bit of the `(i/8)`-th byte represents the interrupt flag of the `i`-th subdevice


## `EXPANDUINO_CMD_META_NUM_SUBDEVICES`
Returns the total number of subdevice in the expanduino (Including the mandatory meta subdevice)
- Parameters: None
- Response: 
    - `numSubdevices`: 1 byte, max 16


## `#EXPANDUINO_CMD_META_DEVICE_TYPE`
Returns the device type of the `n`-th subdevice
- Parameters:
  - `n`: 1 byte, max numSubdevices-1
- Response:
  - `type`: 1 byte, enum `ExpanduinoSubdeviceType`

 
## `EXPANDUINO_CMD_META_VENDOR_NAME`
Returns this device's vendor name
- Parameters: None
- Response:
  - `name`: UTF-8 encoded string

## `EXPANDUINO_CMD_META_PRODUCT_NAME`
Returns this device's model name
- Parameters: None
- Response:
  - `name`: UTF-8 encoded string

## `EXPANDUINO_CMD_META_SERIAL_NUMBER`
Returns the serial number (Or empty string)
- Parameters: None
- Response:
  - `name`: UTF-8 encoded string
