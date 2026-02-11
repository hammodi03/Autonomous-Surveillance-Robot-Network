#include "Space.h"

Space::Space() :
    xLow(0),
    xHigh(0),
    yLow(0),
    yHigh(0),
    room(0)
{
}

Space::Space(double xLow, double xHigh, double yLow, double yHigh, int room) : 
    xLow(xLow), xHigh(xHigh), yLow(yLow), yHigh(yHigh), room(room) {}

void Space::setXLow(double xLow){
    this->xLow = xLow; 
}
double Space::getXLow(){
    return xLow; 
}

void Space::setXHigh(double xHigh){
    this->xHigh = xHigh; 
}
double Space::getXHigh(){
    return xHigh; 
}

void Space::setYLow(double yLow){
    this->yLow = yLow; 
}
double Space::getYLow(){
    return yLow; 
}

void Space::setYHigh(double yHigh){
    this->yHigh = yHigh; 
}
double Space::getYHigh(){
    return yHigh; 
}

void Space::setRoomNr(int room){
    this->room = room; 
}

int Space::getRoomNr() const{
    return room; 
}


