#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <Arduino.h>

class IrSensor {
private:
  int irPin;
  int irValue; 

public:
  IrSensor(int pin);
  void setupIrSensor();
  void update();
  int getIrValue();
};


#endif
