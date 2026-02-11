#include "Movement.h"
#include "Position.h"

/*
  Y är framåt respektive bakåt medan x är åt höger respektive vänster.
  Om man inte vill ha den dynamiska stepSize utan bara att man ska flytta sig framåt 1 steg, kan man bara kommentera ut anropen till
  setStepSize().
*/

Movement::Movement() {
}

void Movement::setupMovement() {
  x = 1.0;
  y = 1.0;

  //printPosition("Initial position: x=");
}

void Movement::stopMoving() {
  firstMovingIteration = true;
}

void Movement::setStepSize() {
  if (firstMovingIteration) {
    movingSecondsSinceLastChecked = millis() / 1000.0;
    firstMovingIteration = false;
  }

  float timeNowSeconds = millis() / 1000.0;
  float movingSeconds = timeNowSeconds - movingSecondsSinceLastChecked;
  movingSecondsSinceLastChecked = timeNowSeconds;

  stepSize = maxSpeed * movingSeconds;
}

double Movement::addErrorMovement(double direction) {
  // Räkna ut direction mellan +-2 grader
  long negativePositive = 0;

  negativePositive = (random(-100, 101)) / 100;

  direction = direction + (negativePositive * 2);

  // Räkna ut felet på hjulens sista varv, encodern

  double distanceOneSpin = 0.0;
  // Om hjulet snurrar mindre än 1 varv är felet försumbart
  distanceOneSpin = PI * wheelDiameter;
  //Serial.print("DistanceOneSpin: ");
  //Serial.println(distanceOneSpin);
  double errorRate = 2.0 / 360.0;
  //Serial.print("errorRate");
  //Serial.println(errorRate);
  stepSize += (distanceOneSpin * negativePositive * errorRate);

  return direction;
}

void Movement::moveForward() {
  setStepSize();
  //Serial.println(stepSize);
  y += stepSize;
  printPosition("Moved forward to: x=");
}

void Movement::moveBackward() {
  setStepSize();
  //Serial.println(stepSize);
  y -= stepSize;
  printPosition("Moved backward to: x=");
}

void Movement::clearforNewMovement() {
  firstMovingIteration = true;
  movingSecondsSinceLastChecked = 0;
}

void Movement::moveDirection(double direction, Position* positioning) {
  if (!(direction < 0)) {
    double robotX = positioning->getXPosition();
    double robotY = positioning->getYPosition();

    setStepSize();

    double errorDirection = addErrorMovement(direction);
    float radian = (errorDirection * PI) / 180.0;

    robotY += cos(radian) * stepSize;
    robotX += sin(radian) * stepSize;  // Där stepSize är storleken på hur mycket man vill flytta sig i diagonal riktning

    positioning->updateXPosition(robotX);
    positioning->updateYPosition(robotY);
  } else {
    setStepSize();
  }
}

void Movement::printPosition(String string) {
  //Serial.print(string);
  //Serial.print(x);
  //Serial.print(", y=");
  //Serial.println(y);
}