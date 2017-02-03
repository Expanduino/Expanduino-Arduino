#include <expanduino.h>
#include <expanduino/transport/i2c.h>
#include <expanduino/arduino/gpio-leds.h>
#include <expanduino/arduino/gpio-linux-input.h>
#include <expanduino/arduino/hwserial.h>


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

ExpanduinoI2C_ ExpanduinoI2C;

ArduinoLed leds[] = {
  {.pin= 2, .name="expanduino:green:led0"},
  {.pin= 3, .name="expanduino:green:led1"},
  {.pin= 4, .name="expanduino:green:led2"},
  {.pin= 5, .name="expanduino:green:led3"},
  {.pin= 8, .name="expanduino:relay0"},
  {.pin= 9, .name="expanduino:relay1"},
};
ExpanduinoSubdeviceGpioLedsArduino expanduinoLeds(ExpanduinoI2C, ARRAY_SIZE(leds), leds);


HardwareSerial* serials[] = {
  &Serial,
  &Serial1,
  &Serial2,
  &Serial3,
};
ExpanduinoSubdeviceHardwareSerialArduino expanduinoHwSerial(ExpanduinoI2C, ARRAY_SIZE(serials), serials);


LinuxInputId input_id = {
  .vendor=0x56,
  .product=0x17,
  .version=0x0101,
};
ArduinoGpioLinuxInputComponent inputs[] = {
  //Analog pins
  {.pin=A0, .type={.type=EV_ABS, .code=ABS_X}},
  {.pin=A1, .type={.type=EV_ABS, .code=ABS_Y}},
  {.pin=A2, .type={.type=EV_ABS, .code=ABS_Z}},
  {.pin=A3, .type={.type=EV_ABS, .code=ABS_THROTTLE}},

  //Push buttons
  
  {.pin=52, .type={.type=EV_KEY, .code=KEY_0}},
  {.pin=50, .type={.type=EV_KEY, .code=KEY_1}},
  {.pin=48, .type={.type=EV_KEY, .code=KEY_2}},
  {.pin=46, .type={.type=EV_KEY, .code=KEY_3}},
  {.pin=44, .type={.type=EV_KEY, .code=KEY_4}},
  {.pin=42, .type={.type=EV_KEY, .code=KEY_5}},
  {.pin=40, .type={.type=EV_KEY, .code=KEY_6}},
  {.pin=38, .type={.type=EV_KEY, .code=KEY_7}},
  {.pin=36, .type={.type=EV_KEY, .code=KEY_8}},
  {.pin=34, .type={.type=EV_KEY, .code=BTN_LEFT}},

  //Buzzer
  {.pin= 7, .type={.type=EV_SND, .code=SND_BELL}}, 

  //Leds
  {.pin= 2, .type={.type=EV_LED, .code=LED_NUML}},
  {.pin= 3, .type={.type=EV_LED, .code=LED_CAPSL}},
  {.pin= 4, .type={.type=EV_LED, .code=LED_SCROLLL}},
  {.pin= 5, .type={.type=EV_LED, .code=LED_COMPOSE}},


  //Relays
  {.pin= 8, .type={.type=EV_LED, .code=LED_MUTE}},
  {.pin= 9, .type={.type=EV_LED, .code=LED_KANA}}

};
ExpanduinoSubdeviceGpioLinuxInputArduino expanduinoLinuxInput(ExpanduinoI2C, input_id, ARRAY_SIZE(inputs), inputs);


void setup() {
  Serial.begin(115200);
  Serial.print("Setup complete");
  ExpanduinoI2C.begin(0x56, 12); 
  //Serial.println(ExpanduinoI2C.);
}

void loop() {
  ExpanduinoI2C.main_loop();
}

