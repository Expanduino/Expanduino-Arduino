#include "keypad-linux-input.h"
#include <Arduino.h>

#define RUN_DELAY 50

ExpanduinoSubdeviceKeypadLinuxInputArduino::ExpanduinoSubdeviceKeypadLinuxInputArduino(Expanduino& container, const char* name, const char* shortName, const LinuxInputId &linuxInputId, int* rows, int numRows, int* cols, int numCols, ArduinoKeypadLinuxInputComponent* components)
: ExpanduinoSubdeviceLinuxInput(container, name, shortName),
  linuxInputId(linuxInputId),
  rows(rows),
  numRows(numRows),
  cols(cols),
  numCols(numCols),
  components(components)
{
}

void ExpanduinoSubdeviceKeypadLinuxInputArduino::begin() {
  for (int i=0; i<numRows*numCols; i++) {
    components[i].actualValue = 0;
    components[i].knownValue = 0;
  }
  for (int row=0; row<numRows; row++) {
    pinMode(rows[row], INPUT_PULLUP);
  }
  for (int col=0; col<numCols; col++) {
    pinMode(cols[col], INPUT_PULLUP);
  }
  currentRow = 0;
  rowEnabled = false;
  scheduler.schedule(this);
}

void ExpanduinoSubdeviceKeypadLinuxInputArduino::end() {
  scheduler.removeCallbacks(this);
  for (uint8_t i=0; i<numRows*numCols; i++) {
    components[i].actualValue = 0;
    components[i].knownValue = 0;
  }
  for (uint8_t row=0; row<numRows; row++) {
    pinMode(rows[row], INPUT);
  }
  for (uint8_t col=0; col<numCols; col++) {
    pinMode(cols[col], INPUT);
  }
}

void ExpanduinoSubdeviceKeypadLinuxInputArduino::run() {
  ArduinoKeypadLinuxInputComponent* comp=&components[currentRow*numCols];
  if (!rowEnabled) {
    pinMode(rows[currentRow], OUTPUT);
    digitalWrite(rows[currentRow], LOW);    
    rowEnabled = true;
  } else {
    bool interrupt = false;
    for (uint8_t col=0; col<numCols; col++, comp++) {
      comp->actualValue = (digitalRead(cols[col]) == LOW);
      if (comp->actualValue != comp->knownValue) {
        interrupt = true;
      }
    }
    
    pinMode(rows[currentRow], INPUT_PULLUP);
    rowEnabled = false;
    currentRow++;
    if (currentRow == numRows) {
      currentRow = 0;
    }

    if (interrupt) {
      noInterrupts();
      requestInterruption();
      interrupts();
    }
  }
  scheduler.scheduleDelayed(this, RUN_DELAY);
}

const LinuxInputId& ExpanduinoSubdeviceKeypadLinuxInputArduino::getLinuxInputId() {
  return linuxInputId;
}

uint8_t ExpanduinoSubdeviceKeypadLinuxInputArduino::getNumComponents() {
  return numRows * numCols;
}

LinuxInputComponentType ExpanduinoSubdeviceKeypadLinuxInputArduino::getComponentType(uint8_t componentNum) {
  return components[componentNum].type;
}

int32_t ExpanduinoSubdeviceKeypadLinuxInputArduino::getValue(uint8_t componentNum) {
  ArduinoKeypadLinuxInputComponent& comp = components[componentNum];
  comp.knownValue = comp.actualValue;
  return comp.knownValue;
}

void ExpanduinoSubdeviceKeypadLinuxInputArduino::getInterruptionReport(LinuxInputStateChange* valuesArray, uint8_t &valuesArrayLength) {
  for (uint8_t componentNum=0; (componentNum<this->getNumComponents()) && (valuesArrayLength < LINUX_INPUT_MAX_INTERRUPT_EVENTS); componentNum++) {
    ArduinoKeypadLinuxInputComponent& comp = components[componentNum];
    if (comp.knownValue != comp.actualValue) {
      comp.knownValue = comp.actualValue;
      valuesArray[valuesArrayLength].component = componentNum;
      valuesArray[valuesArrayLength].value = comp.knownValue ? 1 : 0;
      valuesArrayLength++;
    }
  }

  if (valuesArrayLength == LINUX_INPUT_MAX_INTERRUPT_EVENTS) {
    this->requestInterruption();
  }
}

//Not applicable on Keypads
LinuxInputAbsoluteComponentConfig ExpanduinoSubdeviceKeypadLinuxInputArduino::getAbsoluteComponentConfig(uint8_t componentNum) {
  return LinuxInputAbsoluteComponentConfig();
}

void ExpanduinoSubdeviceKeypadLinuxInputArduino::setValue(uint8_t componentNum, int32_t value) {
}

