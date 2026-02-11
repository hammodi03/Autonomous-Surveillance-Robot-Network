#include "Passage.h"


Passage::Passage(int passageDirection, double passageCoordLow, double passageCoordHigh, double crossCoord, int room1, int room2) :
    passageDirection(passageDirection), passageCoordLow(passageCoordLow), passageCoordHigh(passageCoordHigh), crossCoord(crossCoord), room1(room1), room2(room2) {}

int Passage::getPassageDirection(){
    return passageDirection; 
}

double Passage::getPassageCoordLow(){
    return passageCoordLow; 
}

double Passage::getPassageCoordHigh(){
    return passageCoordHigh; 
}

double Passage::getCrossCoord(){
    return crossCoord; 
}

int Passage::getRoom1(){
    return room1; 
}
int Passage::getRoom2(){
    return room2; 
}
int Passage::getRoom3(){
    return room3; 
}
int Passage::getRoom4(){
    return room4; 
}
int Passage::getRoom5(){
    return room5; 
}
int Passage::getRoom6(){
    return room6; 
}
int Passage::getRoom7(){
    return room7; 
}
int Passage::getRoom8(){
    return room8; 
}
int Passage::getRoom9(){
    return room9; 
}
int Passage::getRoom10(){
    return room10; 
}