#include "Person.h"

Person::Person()
    : id(0),
      x(0.0),
      y(0.0),
      roomNbr(0)
{
}

// Getters
int Person::getId() const { return id; }
double Person::getX() const { return x; }
double Person::getY() const { return y; }
int Person::getRoom() const { return roomNbr; }

// Setters
void Person::setId(int val) { id = val; }
void Person::setX(double val) { x = val; }
void Person::setY(double val) { y = val; }
void Person::setRoom(int val) { roomNbr = val; }
