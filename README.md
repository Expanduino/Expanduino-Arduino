Arduinos are pretty often used as I/O controllers for Raspberry Pis and other computers.

But there is no easy way to use your shiny new peripheral - Most of the time you will end writing a driver program that talks your made-up protocol over the serial port.

# Expanduino

Expanduino makes peripheral development easier:
- Provides easy-to-use arduino libraries that you can use
- Specifies a communication protocol (which you don't really need to know about)
- Provides modules for linux, so that you new device is shown in /dev and can be used by any application

# Device types:

Expanduino currently supports these types of devices:
- Input devices
- Serial Ports
- LEDs
- Character LCDs

**Your device can implement many different device types at the same time!**

# Hardware Interface

The Expanduino protocol is meant to be useable with many hardware interfaces.

Right now, only I2C is supported, but Serial, SPI, USB, CAN and Network (TCP Sockets, WebSockets) are planned

# Status

This project is in early development stage.

Many of features already work, but the codebase is messy and changing quickly

There isn't a kernel driver yet - For now, I'm using an [userspace drivers](https://github.com/Expanduino/Expanduino-Python) to attach the device.
