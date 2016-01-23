# Expanduino I2C Protocol

The Expanduino protocol, although transport-agnostic, was mainly design to for I2C devices.

# Function calls

Most calls are made of two I2C operatios (This is a SMBUS `block_process_call()`):
- Write the request: `S Addr Wr [A] Cmd [A] RequestDataLength [A] RequestData [A] ...`
- Read the response: `S Addr Rd [A] [ResponseDataLength] A [ResponseData] ... A P`

Where `Cmd` is composed of sub-device id (upper nibble) and opcode (lower nibble).
The meaning and size of `RequestData` and `ResponseData` depends on the `Cmd` used - See the documentation for your device type.

Of course, if the function you are calling doesn't require argument or doesn't provide a response, you may simplify the I2C communication:

If no arguments are necessary, you may skip the count and data fields (A `read_block_data()` SMBUS operation):
- Write the request: `S Addr Wr [A] Cmd [A]`
- Read the response: `S Addr Rd [A] [DataLength] A [Data] ... A P`

If no response is necessary, you may skip read operation (A `write_block_data()` SMBUS operation):
- Write the request: `S Addr Wr [A] Cmd [A] DataLength [A] Data [A] ...`

If you don't need neither parameters or responses, the `Cmd` will be enough (A `write_byte()` SMBUS operation):
- Write the request: `S Addr Wr [A] Cmd [A]`



# Interrupts

Interrupts happen through an active-low open-drain pin.

The interrupt line might be shared with other devices.

When an interrupt happens, you should call EXPANDUINO_CMD_META_GET_INTERRUPTS on the meta subdevice to check which subdevices require attention.
