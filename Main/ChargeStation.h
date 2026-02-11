#ifndef CHARGESTATION_H
#define CHARGESTATION_H

#include "Space.h"

class ChargeStation : public Space {
private:
  int id;
  bool charging; 

public:
  ChargeStation(double xLow, double xHigh, double yLow, double yHigh, int room, int id);

  void setId(int id);
  int getId();

  void setCharging(bool charging);
  bool isCharging() const;
};

#endif
