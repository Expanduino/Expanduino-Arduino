#include <expanduino.h>
#include <expanduino/transport/i2c.h>
#include <expanduino/arduino/gpio-leds.h>
#include <expanduino/arduino/gpio-linux-input.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

ArduinoLed leds[] = {
  {.pin= 9, .name="expanduino:red:gpio9"},
  {.pin=10, .name="expanduino:green:gpio10"}
};
ExpanduinoSubdeviceGpioLedsArduino expanduinoLeds(ExpanduinoI2C, ARRAY_SIZE(leds), leds);


LinuxInputId input_id = {
  .vendor=0x56,
  .product=0x17,
  .version=0x0101,
};
ArduinoGpioLinuxInputComponent inputs[] = {
  {.pin= 5, .type={.type=EV_SND, .code=SND_BELL}},
  {.pin= 7, .type={.type=EV_ABS, .code=ABS_X}},
  {.pin= 8, .type={.type=EV_ABS, .code=ABS_Y}},
  {.pin= 9, .type={.type=EV_LED, .code=LED_NUML}},
  {.pin=10, .type={.type=EV_LED, .code=LED_CAPSL}}
};
ExpanduinoSubdeviceGpioLinuxInputArduino expanduinoLinuxInput(ExpanduinoI2C, input_id, ARRAY_SIZE(inputs), inputs);

void setup() {
  Serial.begin(9600);
   
  ExpanduinoI2C.begin(0x56, A9);
}

void loop() {
  Serial.print(".");
  delay(500);
}

