#include <expanduino.h>
#include <expanduino/transport/i2c.h>
#include <expanduino/arduino/leds.h>

ArduinoLed leds[] = {
  {.pin= 9, .name="expanduino:red:gpio9"},
  {.pin=10, .name="expanduino:green:gpio10"}
};
ExpanduinoSubdeviceLedsArduino expanduinoLeds(ExpanduinoI2C, sizeof(leds)/sizeof(*leds), leds);


void setup() {
  ExpanduinoI2C.begin(0x56, A9);
}

void loop() {
  // put your main code here, to run repeatedly:

}
