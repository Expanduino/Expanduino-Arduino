#include <expanduino.h>
#include <expanduino/transport/i2c.h>
#include <expanduino/arduino/leds.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

ArduinoLed leds[] = {
  {.pin= 9, .name="expanduino:red:gpio9"},
  {.pin=10, .name="expanduino:green:gpio10"}
};
ExpanduinoSubdeviceLedsArduino expanduinoLeds(ExpanduinoI2C, ARRAY_SIZE(leds), leds);


void setup() {
  ExpanduinoI2C.begin(0x56, A9);
}

void loop() {
  // put your main code here, to run repeatedly:

}
