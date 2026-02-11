#include "ChargeStation.h"


ChargeStation::ChargeStation(double xLow, double xHigh, double yLow, double yHigh, int room, int id)
  : Space(xLow, xHigh, yLow, yHigh, room), id(id), charging(false)
{
}

void ChargeStation::setId(int id) {
  this->id = id;
}

int ChargeStation::getId() {
  return id;
}

void ChargeStation::setCharging(bool charging) {
  this->charging = charging;
}

bool ChargeStation::isCharging() const {
  return charging;
}
