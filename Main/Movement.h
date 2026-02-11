#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>

class Position;

class Movement {
private:
    void test();
    float x;
    float y;
    float stepSize = 1.0;
    bool firstMovingIteration = true;

    float const maxSpeed = 0.67;
    float const wheelDiameter = 0.06;

    double movingSecondsSinceLastChecked = 0.0;
    void setStepSize();
    void printPosition(String string);

public:
    Movement();
    void setupMovement();
    void moveForward();
    void moveBackward();
    void moveDirection(double direction, Position* positioning);
    void stopMoving();
    void clearforNewMovement();
    double addErrorMovement(double direction);
};

#endif
