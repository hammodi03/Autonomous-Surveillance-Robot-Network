#include "Room.h"
#include <Arduino.h>

Room::Room():Space(0,0,0,0,0) {}

Room::Room(double xLow, double xHigh, double yLow, double yHigh, int roomNr):
    Space(xLow, xHigh, yLow, yHigh, roomNr){}

std::vector<BlockedSpaceBlock> Room::getBlockedSpaceBlocks()const{
    return blockedSpaceBlocks;
}

std::vector<ValuableObject> Room::getValuableObjects() const{
    return valuableObjects; 
}

ValuableObject* Room::getValuableObject(int id)
{
  if (this->valuableObjectsToNotRemove.empty()){
        return nullptr; 
    }
    ValuableObject* returnObject; 
      for(size_t i = 0; i < valuableObjectsToNotRemove.size(); i++){
        if(valuableObjectsToNotRemove[i].getId() == id){
            returnObject = &valuableObjectsToNotRemove[i]; 
            return returnObject; 
        }
    }
    return nullptr; 
}

std::vector<Passage>Room::getPassages() const{
    return passages; 
}

int Room::getRoomNbr() const {
    return getRoomNr();
}

void Room::addValuableObject(ValuableObject valuableObject){
    this->valuableObjects.push_back(valuableObject); 
    this->valuableObjectsToNotRemove.push_back(valuableObject);
}

bool Room::checkPresenceValuableObject(int id){
    bool valuableObjectPresent = false; 
    if (this->valuableObjects.empty()){
        return valuableObjectPresent; 
    }
     for(size_t i = 0; i < valuableObjects.size(); i++){
        if(valuableObjects[i].getId() == id){
            valuableObjectPresent = true; 
            return valuableObjectPresent; 
        }
    }
    return valuableObjectPresent; 
}

bool Room::checkPresenceValuableObject(std::string name){
    bool valuableObjectPresent = false; 
    if (this->valuableObjects.empty()){
        return valuableObjectPresent; 
    }
     for(size_t i = 0; i < valuableObjects.size(); i++){
        if(valuableObjects[i].getName() == name){
            valuableObjectPresent = true; 
            return valuableObjectPresent; 
        }
    }
    return valuableObjectPresent; 
}

ValuableObject Room::removeValuableObject(std::string name){
    if (this->valuableObjects.empty()){
        return ValuableObject(); 
    }
    ValuableObject returnObject; 
      for(size_t i = 0; i < valuableObjects.size(); i++){
        if(valuableObjects[i].getName() == name){
            returnObject = valuableObjects[i]; 
            this->valuableObjects.erase(valuableObjects.begin() + i); 
            return returnObject; 
        }
    }
    return returnObject; 
}

ValuableObject Room::removeValuableObject(int id){
    if (this->valuableObjects.empty()){
        return ValuableObject(); 
    }
    ValuableObject returnObject; 

    for(size_t i = 0; i < valuableObjects.size(); i++){
        if(valuableObjects[i].getId() == id){
            returnObject = valuableObjects[i]; 
            this->valuableObjects.erase(valuableObjects.begin() + i); 
            return returnObject; 
        }
    }
    return returnObject; 
}

void Room::addBlockedSpaceBlock(BlockedSpaceBlock blockedSpaceBlock){
    blockedSpaceBlocks.push_back(blockedSpaceBlock); 
}

bool Room::removeBlockedSpaceBlock(double x, double y){
    bool isRemoved = false; 
    if (this->blockedSpaceBlocks.empty()){
        return isRemoved; 
    }
     for(size_t i = 0; i < blockedSpaceBlocks.size(); i++){
        if(x >= blockedSpaceBlocks[i].getXLow() && x <= blockedSpaceBlocks[i].getXHigh() && y >= blockedSpaceBlocks[i].getYLow() && y <= blockedSpaceBlocks[i].getYHigh()){
            this->blockedSpaceBlocks.erase(blockedSpaceBlocks.begin() + i); 
            isRemoved = true; 
            return isRemoved; 
        }
    }
    return isRemoved; 
}

void Room::addPassage(Passage passage){
    passages.push_back(passage); 
}

bool Room::removePassage(int room1, int room2, double passageCoordLow){
    bool passageRemoved = false; 
    if(this->passages.empty()){
        return passageRemoved; 
    }
    for(size_t i = 0; i <passages.size(); i++){
        if(passages[i].getRoom1() == room1 && passages[i].getRoom2() == room2 && passages[i].getPassageCoordLow() == passageCoordLow){
            this->passages.erase(passages.begin() + i); 
            passageRemoved = true; 
            return passageRemoved;          
        }
    }
    return passageRemoved; 
}
