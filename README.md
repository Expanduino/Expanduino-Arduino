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
