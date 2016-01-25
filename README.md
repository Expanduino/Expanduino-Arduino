Arduinos are pretty often used as I/O controllers for Raspberry Pis and other computers.

But there is no easy way to use your shiny new peripheral - Most of the time you will end writing a driver program that talks your made-up protocol over the serial port.

# Expanduino

Expanduino makes peripheral development easier:
- Provides easy-to-use arduino libraries that you can use
- Specifies a communication protocol (which you don't really need to know about)
- Provides modules for linux, so that you new device is shown in /dev and can be used by any application

# Device types:

Expanduino currently supports these types of devices:
- LEDs
- Input devices (Based either linux subsystem or on HID) - TODO
- Character LCDs - TODO
- Serial Ports - TODO
- I2C Ports - TODO
- SPI Ports - TODO

**Your device can implement many different device types at the same time!**

# Hardware Interface

The Expanduino protocol can be used on top of many hardware interfaces:
- I2C
- Serial (TODO)
- USB (TODO)
- Network (TCP Sockets, WebSockets) - TODO
