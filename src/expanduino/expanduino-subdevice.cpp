#include "expanduino.h"

ExpanduinoSubdevice::ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType, const char* name, const char* shortName) 
: container(container),
  devType(devType),
  name(name),
  shortName(shortName),
  interruptionEnabled(false),
  interrupted(false),
  next(nullptr),
  devNum(0)
{
  //Adiciona este componente na lista ligada
  ExpanduinoSubdevice* lastSubdevice = &container.metaSubdevice;
  if (this != lastSubdevice) { //Não executa no metaSubdevice
    while (lastSubdevice->next) {
      lastSubdevice = lastSubdevice->next;
    }
    lastSubdevice->next = this;
    this->devNum = lastSubdevice->devNum + 1;
  }
}

void ExpanduinoSubdevice::begin() {
}

void ExpanduinoSubdevice::end() {
}

void ExpanduinoSubdevice::reset() {
}

void ExpanduinoSubdevice::pool() {
}

void ExpanduinoSubdevice::getName(Print& out) {
  out.print(name);
}

void ExpanduinoSubdevice::getShortName(Print& out) {
  out.print(shortName);
}

void ExpanduinoSubdevice::setInterruptionEnabled(bool enabled) {
  this->interruptionEnabled = enabled;
  if (!enabled) {
    this->interrupted = false;
    this->clearInterruption();
  }
}

bool ExpanduinoSubdevice::requestInterruption() {
  return this->container.requestInterruption(this);
}

void ExpanduinoSubdevice::clearInterruption() {
  // should be overriden on subclasses that support interruptions
}

void ExpanduinoSubdevice::startedInterruption() {
  // should be overriden on subclasses that support interruptions
}

void ExpanduinoSubdevice::stoppedInterruption() {
  // should be overriden on subclasses that support interruptions
}

void ExpanduinoSubdevice::readInterruptionData(Print& response) {
  // should be overriden on subclasses that support interruptions
}