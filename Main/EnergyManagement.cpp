#include "EnergyManagement.h"

EnergyManagement::EnergyManagement() {
}

float EnergyManagement::getPercentage() {
  return percentageLeft;
}

void EnergyManagement::setPercentage(float newPercentage) {
  if (newPercentage < 101 && newPercentage > -1) {
    percentageLeft = newPercentage;
    mAhLeft = (percentageLeft / 100.0f) * totalmAh;
  }
}

void EnergyManagement::setmAh(int newmAh) {
  mAhLeft = newmAh;
}

void EnergyManagement::addToCommunicationDrain() {
  int drain = 1;
  xQueueSend(communicationQueueDrain, &drain, 0);
}

void EnergyManagement::turnOffDraining() {
  //Serial.println(energyDrain(true, false, false));
  firstDrainingIteration = true;
}

void EnergyManagement::turnOffCharging() {
  firstChargingIteration = true;
}

float EnergyManagement ::charging() {
  if (firstChargingIteration) {
    chargingHoursSinceLastChecked = (millis() / 1000.0) / 3600.0;
    firstChargingIteration = false;
  }

  if (percentageLeft < 100) {
    float timeNowHours = (millis() / 1000.0) / 3600.0;
    float chargingHours = timeNowHours - chargingHoursSinceLastChecked;
    chargingHoursSinceLastChecked = timeNowHours;
    /*
      Adderar uppladdningen som skett till det förgående antalet procent som fanns kvar i batteriet. 
      Multiplicerar med laddningsström, antalet h det laddat och 1000. Multiplicerar med 1000 för att få det i mAh 
    */
    mAhLeft += chargingHours * charging_I * 1000;
    if (mAhLeft > totalmAh) { mAhLeft = totalmAh; }
    percentageLeft = (mAhLeft / totalmAh) * 100;
  }
  //Serial.print("Percentage left: ");
  //Serial.println(percentageLeft);
  return percentageLeft;
}

float EnergyManagement ::energyDrain(bool moving, bool communicating, bool detecting) {
  if (firstDrainingIteration) {
    chargingHoursSinceLastChecked = (millis() / 1000.0) / 3600.0;
    firstChargingIteration = false;
  }

  if (percentageLeft > 0) {
    float timeNowHours = (millis() / 1000.0) / 3600.0;
    float activeHours = timeNowHours - activeHoursSinceLastChecked;
    float totalDrain = 0;
    activeHoursSinceLastChecked = timeNowHours;

    /* 
      Kan istället för bool använda vilket state den är inne i här för att avgöra hur mycket drain är
      Använder inte if-else eftersom man kan vara i moving och communicating samtidigt 
    */

    if (moving) {
      totalDrain += movingDrain;
    }
    if ((uxQueueMessagesWaiting(communicationQueueDrain) != 0) || communicating) {
      int receivedDrain = 0;
      while (xQueueReceive(communicationQueueDrain, &receivedDrain, 0) == pdTRUE) {
        totalDrain += communicatingDrain;
      }
      if (communicating) { totalDrain += communicatingDrain; }
    }
    if (detecting) {
      totalDrain += detectingDrain;
    }

    mAhLeft -= activeHours * totalDrain * 1000.0;
    if (mAhLeft < 0) { mAhLeft = 0; }
    percentageLeft = (mAhLeft / totalmAh) * 100;
  }

  return percentageLeft;
}