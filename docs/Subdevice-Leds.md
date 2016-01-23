# Leds subdevice

The leds subdevice connects to Linux's LED subsystem and registers any number of LED Devices. Each new device becomes a new entry on `/sys/class/leds`

This is probably the easiest subdevice to implement - Just a led, a resistor and a free GPIO, and you are done.

Of course, you can get as fancy as you want, with PWMs, RGB leds and even Neopixels!

# Operations

## `EXPANDUINO_CMD_LED_NUM_LEDS`
Returns the number of leds

- Parameters: None
- Response: 
  - `numLeds`: 1 byte


## `EXPANDUINO_CMD_LED_NAME`
Returns the name of the `n`-th led
- Parameters:
  - `n`: 1 byte, max numLeds-1
- Response: 
  - `name`: UTF-8 encoded string


## `EXPANDUINO_CMD_LED_GET_BRIGHTNESS`

Returns the current brightness value.

- Parameters:
  - `ledNum`: 1 byte, max numLeds-1
- Response: 
  - `brightness`: 1 byte, max 255

## `EXPANDUINO_CMD_LED_SET_BRIGHTNESS`

Sets the brightness of the `n`-th led to the new value.

Returns the previous brightness value.

- Parameters:
  - `ledNum`: 1 byte, max numLeds-1
  - `brightness`: 1 byte, max 255
- Response: None
