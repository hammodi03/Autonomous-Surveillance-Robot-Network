#include "Goal.h"

Goal::Goal():goalX(0), goalY(0), goalRoom(0), isZone(false), isComZone(false){}
Goal::Goal(double goalX, double goalY, int goalRoom, bool isZone, bool isComZone) :
    goalX(goalX), goalY(goalY), goalRoom(goalRoom), isZone(isZone), isComZone(isComZone) {}

void Goal::setGoalX(double goalX){
    this->goalX = goalX; 
}
double Goal::getGoalX(){
    return this->goalX; 
}
void Goal::setGoalY(double goalY){
    this->goalY = goalY; 
}

double Goal::getGoalY(){
    return this->goalY; 
}

void Goal::setRoom(int room){
    this->goalRoom = room; 
}
int Goal::getRoom(){
    return this->goalRoom; 
}

void Goal::setZone(bool zone) {
    this->isZone = zone;
}

bool Goal::getIsZone() {
    return this->isZone;
}

void Goal::setIsComZone(bool isComZone) {
    this->isComZone = isComZone;
}

bool Goal::getIsComZone() {
    return this->isComZone;
}