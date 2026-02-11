#ifndef AGENT_H
#define AGENT_H

#include <Arduino.h>
#include "ActivityPlanning.h"

class Agent {
private:
    int id;
    double x;
    double y;
    int roomNbr;
    int actualCurrentRoom;
    int groupId;
    int nextId;
    bool atCommunicationGoal;
    bool goalSetToCharger;
    double percentage;
    unsigned long lastMsgTime;

public:
    Agent();

    // Getters
    int getId() const;
    double getX() const;
    double getY() const;
    int getRoom() const;
    int getActualRoom() const;
    int getGroupId() const;
    int getNextId() const;
    bool getAtComGoal() const;
    bool getGoalSetToCharger();
    double getPercentage();
    unsigned long getLastMsgTime() const;

    // Setters
    void setId(int val);
    void setX(double val);
    void setY(double val);
    void setRoom(int val);
    void setActualRoom(int val);
    void setGroupId(int val);
    void setNextId(int val);
    void setAtComGoal(bool val);
    void setGoalSetToCharger(bool val);
    void setPercentage(double val);
    void setLastMsgTime(unsigned long val);
};

#endif
