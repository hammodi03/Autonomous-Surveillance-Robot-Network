#include "ValuableObject.h"
#include <Arduino.h>

ValuableObject::ValuableObject()
  : Space(0, 0, 0, 0, 0), name(""), id(0), stolen(false), isZone(false), isComZone(false) {
}

ValuableObject::ValuableObject(double xLow, double xHigh, double yLow, double yHigh,
                               int room,
                               const std::string& name,
                               int id,
                               bool stolen,
                               bool isZone,
                               bool isComZone)
  : Space(xLow, xHigh, yLow, yHigh, room),
    name(name),
    id(id),
    stolen(stolen) {
}

// Name
void ValuableObject::setName(const std::string& name) {
  this->name = name;
}

std::string ValuableObject::getName() const {
  return name;
}

void ValuableObject::setId(int id) {
  this->id = id;
}

int ValuableObject::getId() const {
  return id;
}

void ValuableObject::setStolen(bool stolen) {
  this->stolen = stolen;
}

bool ValuableObject::isStolen() const {
  return stolen;
}

void ValuableObject::setZone(bool isZone) {
  this->isZone = isZone;
}

bool ValuableObject::getIsZone() const {
  return isZone;
}

void ValuableObject::setComZone(bool isComZone) {
  this->isComZone = isComZone;
}

bool ValuableObject::getIsComZone() const {
  return isComZone;
}
