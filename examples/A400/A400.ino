#include <expanduino.h>
#include <expanduino/transport/i2c.h>
#include <expanduino/arduino/gpio-leds.h>
#include <expanduino/arduino/gpio-lcd.h>
#include <expanduino/arduino/gpio-linux-input.h>
#include <expanduino/arduino/keypad-linux-input.h>
#include <expanduino/arduino/hwserial.h>
#include <expanduino/arduino/wiegand-serial.h>


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

ExpanduinoI2C_ ExpanduinoI2C;

LinuxInputId input_id = {
  .vendor = 0x56,
  .product = 0x17,
  .version = 0x0101,
};

ArduinoGpioLinuxInputComponent digital_in[] = {
  {PIN_PK0, {.type = EV_KEY, .code = KEY_F1}},
  {PIN_PK1, {.type = EV_KEY, .code = KEY_F2}},
  {PIN_PK2, {.type = EV_KEY, .code = KEY_F3}},
  {PIN_PK3, {.type = EV_KEY, .code = KEY_F4}},
  {PIN_PK4, {.type = EV_KEY, .code = KEY_F5}},
  {PIN_PK5, {.type = EV_KEY, .code = KEY_F6}},
  {PIN_PK6, {.type = EV_KEY, .code = KEY_F7}},
  {PIN_PK7, {.type = EV_KEY, .code = KEY_F8}}
};
ExpanduinoSubdeviceGpioLinuxInputArduino expanduinoDigitalIn(ExpanduinoI2C, "Digital Inputs", "digital_in", input_id, digital_in, ARRAY_SIZE(digital_in));



ArduinoGpioLinuxInputComponent relays[] = {
  {.pin = PIN_PC0, .type = {.type = EV_LED, .code = 0}},
  {.pin = PIN_PC1, .type = {.type = EV_LED, .code = 1}},
  {.pin = PIN_PC2, .type = {.type = EV_LED, .code = 2}},
  {.pin = PIN_PC3, .type = {.type = EV_LED, .code = 3}},
  {.pin = PIN_PC4, .type = {.type = EV_LED, .code = 4}},
  {.pin = PIN_PC5, .type = {.type = EV_LED, .code = 5}},
  {.pin = PIN_PC6, .type = {.type = EV_LED, .code = 6}},
  {.pin = PIN_PC7, .type = {.type = EV_LED, .code = 7}}
};
ExpanduinoSubdeviceGpioLinuxInputArduino expanduinoRelays(ExpanduinoI2C, "Relays", "relays", input_id, relays, ARRAY_SIZE(relays));



ArduinoGpioLinuxInputComponent pictogram[] = {
  {.pin = PIN_PH3, .type = {.type = EV_LED, .code = 0}},
  {.pin = PIN_PH4, .type = {.type = EV_LED, .code = 1}},
  {.pin = PIN_PH5, .type = {.type = EV_LED, .code = 2}},
  {.pin = PIN_PH6, .type = {.type = EV_LED, .code = 3}},
  {.pin = PIN_PL4, .type = {.type = EV_LED, .code = 4}},
  {.pin = PIN_PL5, .type = {.type = EV_LED, .code = 5}},
};
ExpanduinoSubdeviceGpioLinuxInputArduino expanduinoPicto(ExpanduinoI2C, "Pictogram", "pictogram", input_id, pictogram, ARRAY_SIZE(pictogram));



ArduinoGpioLinuxInputComponent sound[] = {
  {.pin = PIN_PG5, .type = {.type = EV_SND, .code = SND_BELL}},
};
ExpanduinoSubdeviceGpioLinuxInputArduino expanduinoSnd(ExpanduinoI2C, "Buzzer", "buzzer", input_id, sound, ARRAY_SIZE(sound));



int keypad_rows[] = {PIN_PA4, PIN_PA5, PIN_PA6, PIN_PA7};
int keypad_cols[] = {PIN_PA0, PIN_PA1, PIN_PA2, PIN_PA3};
ArduinoKeypadLinuxInputComponent keypad[] = {
  {.type = {.type = EV_KEY, .code = KEY_1}},
  {.type = {.type = EV_KEY, .code = KEY_2}},
  {.type = {.type = EV_KEY, .code = KEY_3}},
  {.type = {.type = EV_KEY, .code = KEY_A}},

  {.type = {.type = EV_KEY, .code = KEY_4}},
  {.type = {.type = EV_KEY, .code = KEY_5}},
  {.type = {.type = EV_KEY, .code = KEY_6}},
  {.type = {.type = EV_KEY, .code = KEY_B}},

  {.type = {.type = EV_KEY, .code = KEY_7}},
  {.type = {.type = EV_KEY, .code = KEY_8}},
  {.type = {.type = EV_KEY, .code = KEY_9}},
  {.type = {.type = EV_KEY, .code = KEY_C}},

  {.type = {.type = EV_KEY, .code = KEY_ESC}},
  {.type = {.type = EV_KEY, .code = KEY_0}},
  {.type = {.type = EV_KEY, .code = KEY_ENTER}},
  {.type = {.type = EV_KEY, .code = KEY_D}}
};
ExpanduinoSubdeviceKeypadLinuxInputArduino expanduinoKeypad(ExpanduinoI2C, "Keypad", "keypad", input_id, keypad_rows, ARRAY_SIZE(keypad_rows), keypad_cols, ARRAY_SIZE(keypad_cols), keypad);



AsyncLiquidCrystal lcd(PIN_PH7, PIN_PG3, PIN_PG4,
                  PIN_PL0, PIN_PL1, PIN_PL2, PIN_PL6);
ExpanduinoSubdeviceGpioLcdArduino expanduinoLcd(ExpanduinoI2C, "LCD", "lcd", lcd, PIN_PL3);



HardwareSerial* serials[] = {
  &Serial,
  &Serial1,
  &Serial2,
  &Serial3,
};
ExpanduinoSubdeviceHardwareSerialArduino expanduinoHwSerial(ExpanduinoI2C, "Serial", "serial", serials, ARRAY_SIZE(serials));



ArduinoWiegandComponent wiegands[] = {
  {.pin0 = PIN_PJ3, .pin1 = PIN_PJ4},
  {.pin0 = PIN_PJ5, .pin1 = PIN_PJ6},
  {.pin0 = PIN_PB4, .pin1 = PIN_PB5},
  {.pin0 = PIN_PB6, .pin1 = PIN_PB7}
};
ExpanduinoSubdeviceWiegandSerialArduino expanduinoWiegand(ExpanduinoI2C, "Wiegand", "wiegand", wiegands, ARRAY_SIZE(wiegands));

void setup() {
  ExpanduinoI2C.vendorName = "Veridis";
  ExpanduinoI2C.productName = "A400";
  ExpanduinoI2C.shortName = "a400";
  ExpanduinoI2C.begin(0x56, PIN_PE3);
}

void loop() {
  ExpanduinoI2C.main_loop();
}
