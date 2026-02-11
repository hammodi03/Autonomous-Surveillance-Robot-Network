#include "IRSensor.h"

IrSensor::IrSensor(int pin) {
  irPin = pin;
  irValue = 0;
}

void IrSensor::setupIrSensor() {
  pinMode(irPin, INPUT_PULLUP);
}

void IrSensor::update() {
  irValue = digitalRead(irPin);
}

int IrSensor::getIrValue() {
  return irValue;
}