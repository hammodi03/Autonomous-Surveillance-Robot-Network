#ifndef GOAL_H
#define GOAL_H

#include <Arduino.h>

struct GoalMessage
{
  float targetX;
  float targetY;
  uint8_t targetRoom;
};

class Goal {
    private: 
        double goalX; 
        double goalY;
        int goalRoom; 
        bool isZone = false;
        bool isComZone = false;

    public: 
        Goal(); 
        Goal(double goalX, double goalY, int goalRoom, bool isZone = false, bool isComZone = false);

        void setGoalX(double goalX); 
        double getGoalX(); 

        void setGoalY(double goalY); 
        double getGoalY(); 

        void setRoom(int room); 
        int getRoom(); 

        void setZone(bool zone);
        bool getIsZone();

        void setIsComZone(bool isComZone);
        bool getIsComZone();
}; 

#endif