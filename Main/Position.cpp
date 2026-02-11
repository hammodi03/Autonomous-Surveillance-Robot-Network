#include "Position.h"


Position::Position(double xPosition, 
    double yPosition, int room) : xPosition(xPosition), yPosition(yPosition), room(room){}

void Position::updateXPosition(double updateX){
    xPosition = updateX; 
}

void Position::updateYPosition(double updateY){
    yPosition = updateY; 
}

void Position::changeXPosition(double changeX){
    xPosition = xPosition + changeX; 
}

void Position::changeYPosition(double changeY){
    yPosition = yPosition + changeY; 
}

void Position::changeId(int newId) {
    id = newId;
}

double Position::getXPosition() const{
    return xPosition; 
}

double Position::getYPosition() const{
    return yPosition; 
}

int Position::getId() const {
    return id;
}

void Position::setLastDirection(double direction){
    lastDirection = direction; 
}

double Position::getLastDirection(){
    return lastDirection; 
}

void Position::updateRoom(int room){
    this->room = room; 
}

int Position::getRoom() const{
    return room; 
}