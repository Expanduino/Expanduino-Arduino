# Expanduino-Arduino
Arduino library that implements Expanduino protocol

You just built yourself a nice Arduino-based peripheral device, and now, you need to communicate with a Linux box?

You can read and write data over the Serial port using your own software (You are probably doing it already).

But if your device is just a bunch of buttons, leds, serial ports, etc, why bother?
There should be a kernel driver to make it work out-of-the box. 

**This is what the expanduino library is there for!**

# Composite devices

An Expanduino device is a composite of several sub-devices

For instance, if you have your arduino connected to a keypad, a bunch of leds, a serial port and an LCD, it will be attached to several subsystems in the linux kernel:
- [Input subsystem](https://github.com/torvalds/linux/blob/master/Documentation/input/input.txt), will create an entry in `/dev/input` to access the keypad.
- [Led class](https://github.com/torvalds/linux/blob/master/Documentation/leds/leds-class.txt), will create entries in `/sys/class/leds/` for each led.
- [Serial driver](https://github.com/torvalds/linux/blob/master/Documentation/serial/driver), will create entries in `/dev/tty***` for each serial port
- [Misc devices](http://www.linuxjournal.com/article/2920) will create `/dev/mything` for all the non-standard components, like LCD displays.


# Basic Protocol

Every requests starts with a "register byte" which identified the device (upper nibble) and the operation (lower nibble).

The special device #0 is reserved for metadata.

The special function #0 resets the device

# Special Device #0

Functions:

1. getNumDevices()
  - Parameters: None
  - Response: number of devices (1 byte)

1. Function #1: getDeviceType()
  - Parameters: device number (1 byte)
  - Response: device type (1 byte)

1. Function #2: getInterrupts()
  - Parameters: None
  - Response: interrupt flags for each sub-device. Device #0 is an OR of all other devices (2 byte))



# LED Device (DeviceType #1)

Functions:

1. getNumLeds()
  - Parameters: None
  - Response: number of leds (1 byte)

1. getLedName()
  - Parameters: led number (1 byte)
  - Response: 32 bytes, Led name

1. getLedBrightness()
  - Parameters: led number (1 byte)
  - Response: brightness  (1 byte)

1. setLedBrightness()
  - Parameters: led number (1 byte), brightness  (1 byte)
  - Response: none



# GPIO Device (DeviceType #2)

Enums:

1. GPIO_FLAGS = `GPIO_OUTPUT | GPIO_INPUT | GPIO_PULLUP | GPIO_PULLDOWN | GPIO_OPEN_DRAIN | GPIO_OPEN_SOURCE | GPIO_PWM | GPIO_ADC | GPIO_DAC | GPIO_INTERRUPT_CHANGE | GPIO_INTERRUPT_RISING | GPIO_INTERRUPT_FALLING | GPIO_INTERRUPT_LOW | GPIO_INTERRUPT_HIGH`

Functions:
1. getNumGpio()
  - Parameters: None
  - Response: number of gpios (1 byte)

1. getGpioCapabilities()
  - Parameters: gpio number (1 byte)
  - Response: flags (2 bytes)

1. setGpioFlags()
  - Parameters: gpio number (1 byte), flags (2 bytes)
  - Response: None

1. digitalRead()
- Parameters: gpio number (1 byte)
- Response: 1 byte, value

1. digitalWrite()
  - Parameters: gpio number (1 byte), value (1 byte)
  - Response: None

1. analogRead()
- Parameters: gpio number (1 byte)
- Response: value (2 bytes)

1. analogWrite()
- Parameters: gpio number (1 byte), value (2 bytes)
- Response: None

1. attachInterrupt()
  - Parameters: gpio number (1 byte), mode (1 byte)
  - Response: None

1. dettachInterrupt()
  - Parameters: gpio number (1 byte), mode (1 byte)
  - Response: None
  
# Input Device (DeviceType #3)
  Maps directly to the Linux Input Subsystem (Easy to use)

# HID Device (DeviceType #4)
  Maps directly to the Linux HID Subsystem (Harder to use, more powerful)

# Serial Port/TTY Device (DeviceType #5)
  Maps directly to the Linux HID Subsystem (Harder to use, more powerful)

# Misc Device (DeviceType #6)
  Something else that isn't mapped to a Linux subsystem.
  A new character device is created on `/dev/mything`, and `read`/`write`/`ioctl` operations are handled on the microcontroller.
