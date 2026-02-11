#ifndef RADAR_H
#define RADAR_H

#include "MyLD2410.h"
#include <HardwareSerial.h>

class Radar {
public:
    Radar(HardwareSerial &serial);

    

    // Initialize the radar
    bool initRadar(bool enhancedMode = true, uint8_t autoThresholdTimeout = 11);

    // Read latest sensor data; returns true if new data is available
    bool readValue();
    // Reset radar values
    void resetValues();
    // Getters/Setters
    unsigned long getDistance() const;
    bool isMovingDetected() const;
    unsigned long getMovingDistance() const;
    byte getMovingSignal() const;
    bool isStationaryDetected() const;
    unsigned long getStationaryDistance() const;
    byte getStationarySignal() const;
    bool getSimulatedStatus();
    void setSimulatedStatus(bool simulated);
    

    //Check if object is in scanning cone (simulation, check other robots)
    bool isUnitInCone(float unitX, float unitY,
                          float coneX, float coneY,
                          float dirDeg, float halfAngleDeg,
                          float maxRange);

private:
    MyLD2410 sensor;
    HardwareSerial &serialPort;

    // Internal storage
    unsigned long distance;
    bool movingDetected;
    unsigned long movingDistance;
    byte movingSignal;
    bool stationaryDetected;
    unsigned long stationaryDistance;
    byte stationarySignal;
    bool simulateRadar;
};

#endif
