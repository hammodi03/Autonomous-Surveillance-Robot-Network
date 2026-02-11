#include "Radar.h"
#include <Arduino.h> // only for delay()
#include <math.h> // for cos(), sin(), sqrt(), radians()

Radar::Radar(HardwareSerial &serial)
: sensor(serial), serialPort(serial),
  distance(0),
  movingDetected(false),
  movingDistance(0),
  movingSignal(0),
  stationaryDetected(false),
  stationaryDistance(0),
  stationarySignal(0),
  simulateRadar(true)
{
}

bool Radar::initRadar(bool enhancedMode, uint8_t autoThresholdTimeout)
{
    serialPort.begin(256000, SERIAL_8N1, 16, 17); // RX 16, TX 17

    if (!sensor.begin()) {  
        return false;
    }
    
    //Auto-Calibration gate thesholds
    if (enhancedMode) {
        sensor.enhancedMode();
        sensor.autoThresholds(autoThresholdTimeout);
        delay(200);
    } else {
        sensor.enhancedMode(true);   // enter engineering mode
        delay(100);
        sensor.autoThresholds(autoThresholdTimeout);
        delay(500);
        sensor.enhancedMode(false);  // back to normal mode
    }
    Serial.println("Radar calibrated.");
    return true;
}



//============================================= Non-simulated scanning function ==========================================================
bool Radar::readValue()
{
    if (sensor.check() == MyLD2410::Response::DATA) {
        distance = sensor.detectedDistance();

        movingDetected = sensor.movingTargetDetected();
        if (movingDetected) {
            movingDistance = sensor.movingTargetDistance();
            movingSignal = sensor.movingTargetSignal();
        }

        stationaryDetected = sensor.stationaryTargetDetected();
        if (stationaryDetected) {
            stationaryDistance = sensor.stationaryTargetDistance();
            stationarySignal = sensor.stationaryTargetSignal();
        }

        return true;
    }
    return false;
}

void Radar::resetValues(){
  stationarySignal = 0;
  movingSignal = 0;
  movingDetected = 0;
  stationaryDetected = 0;
  movingSignal = 0;
  movingDistance = 0;
  stationaryDistance = 0;
}



// Check if a unit at (unitX, unitY) is inside a cone. Can be used for simulation to see if we actually scan something and for communicating if we need someone to move/if we need to wait for someone to move before scanning
//unitX, unitY -> the point to check if it is in tbhe cone. coneX, coneY -> current point from where we are scanning. dirDeg -> direction in degrees we are facing. halfAngleDeg -> 120/2 = 60 degrees. maxRange = 4m
bool Radar::isUnitInCone(float unitX, float unitY,
                          float coneX, float coneY,
                          float dirDeg, float halfAngleDeg,
                          float maxRange)
{
    // Convert direction to radians and get unit vector
    float dirRad = dirDeg * 3.14159265 / 180.0;
    float dirX = cos(dirRad);
    float dirY = sin(dirRad);

    // Vector from cone vertex to unit 
    float vX = unitX - coneX;
    float vY = unitY - coneY;

    // Distance
    float dist = sqrt(vX*vX + vY*vY);
    if (dist > maxRange) return false;//<---- unit is outside the radar cone of influence (Too far)

    // Dot product
    float dot = vX*dirX + vY*dirY;
    float cosTheta = dot / dist; // since dir is unit vector

    // Convert half-angle to radians and check
    float halfRad = halfAngleDeg * 3.14159265 / 180.0;
    return cosTheta >= cos(halfRad); //<--- Returns true if unit is inside the radar cone of influence, false if not
}


// --- Getters/Setters ---
unsigned long Radar::getDistance() const { return distance; }

bool Radar::getSimulatedStatus(){ return simulateRadar; }
void Radar::setSimulatedStatus(bool simulated){ simulateRadar = simulated; }

bool Radar::isMovingDetected() const { return movingDetected; }
unsigned long Radar::getMovingDistance() const { return movingDistance; }
byte Radar::getMovingSignal() const { return movingSignal; }

bool Radar::isStationaryDetected() const { return stationaryDetected; }
unsigned long Radar::getStationaryDistance() const { return stationaryDistance; }
byte Radar::getStationarySignal() const { return stationarySignal; }
