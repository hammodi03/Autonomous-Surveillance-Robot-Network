#ifndef ENERGYMANAGEMENT_H
#define ENERGYMANAGEMENT_H

#include <Arduino.h>
#include "freertos/queue.h"

class EnergyManagement {
private:
  float const totalmAh = 3000 * 0.8; // Batteriets kapacitet där man max vill använda 80%
  // Alla siffror som är konstanter nedan är i Ah
  float const charging_I = 6.6; // Laddningsström
  float const movingDrain = 0.812; // Motorer, motordrivare och magnetometer
  float const communicatingDrain = 0.083; // ESP Now, mitten mellan som max och som lägst
  float const detectingDrain = 0.079; // Radarn

  float mAhLeft = 3000 * 0.8; // Startvärdet för batteriets laddning
  float chargingHoursSinceLastChecked = 0;
  float activeHoursSinceLastChecked = 0;
  float percentageLeft = 100;
  bool firstChargingIteration = true;
  bool firstDrainingIteration = true;

public:
  QueueHandle_t communicationQueueDrain = xQueueCreate(10, sizeof(int));

  EnergyManagement();

  float charging();
  float energyDrain(bool moving, bool communicating, bool detecting);

  float getPercentage();
  void setPercentage(float newPercentage);
  void setmAh(int newmAh);

  void turnOffCharging();
  void turnOffDraining();

  void addToCommunicationDrain();
};


#endif