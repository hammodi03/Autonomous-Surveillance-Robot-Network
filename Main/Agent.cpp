#include "Agent.h"

Agent::Agent()
    : id(0),
      x(0.0),
      y(0.0),
      roomNbr(0),
      groupId(0),
      nextId(0),
      atCommunicationGoal(false),
      goalSetToCharger(false)
{
}

// Getters
int Agent::getId() const { return id; }
double Agent::getX() const { return x; }
double Agent::getY() const { return y; }
int Agent::getRoom() const { return roomNbr; }
int Agent::getActualRoom() const { return actualCurrentRoom; }
int Agent::getGroupId() const { return groupId; }
int Agent::getNextId() const { return nextId; }
bool Agent::getAtComGoal() const { return atCommunicationGoal; }
bool Agent::getGoalSetToCharger() { return goalSetToCharger; }
double Agent::getPercentage(){ return percentage; }
unsigned long Agent::getLastMsgTime() const { return lastMsgTime; }

// Setters
void Agent::setId(int val) { id = val; }
void Agent::setX(double val) { x = val; }
void Agent::setY(double val) { y = val; }
void Agent::setRoom(int val) { roomNbr = val; }
void Agent::setActualRoom(int val) { actualCurrentRoom = val; }
void Agent::setGroupId(int val) { groupId = val; }
void Agent::setNextId(int val) { nextId = val; }
void Agent::setAtComGoal(bool val) { atCommunicationGoal = val; }
void Agent::setGoalSetToCharger(bool val) { goalSetToCharger = val; }
void Agent::setPercentage(double val){ percentage = val; }
void Agent::setLastMsgTime(unsigned long val) { lastMsgTime = val; }
