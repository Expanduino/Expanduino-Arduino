#include "expanduino.h"

ExpanduinoSubdevice::ExpanduinoSubdevice(Expanduino& container, ExpanduinoSubdeviceType devType, const char* name, const char* shortName) 
: container(container),
  devType(devType),
  name(name),
  shortName(shortName),
  isInterrupted(false),
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

void ExpanduinoSubdevice::reset() {
}

void ExpanduinoSubdevice::getName(Print& out) {
  out.print(name);
}

void ExpanduinoSubdevice::getShortName(Print& out) {
  out.print(shortName);
}

void ExpanduinoSubdevice::requestInterrupt() {
  if (!this->isInterrupted) {
    this->isInterrupted = true;
    container.requestInterrupt();
  }
}

bool ExpanduinoSubdevice::clearInterruptStatus() {
  bool ret = this->isInterrupted;
  this->isInterrupted = false;
  return ret;
}
