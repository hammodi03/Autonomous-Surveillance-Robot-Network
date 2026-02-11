#ifndef POSITION_H
#define POSITION_H

#include <cstdint>

class Position {
    
    private:
        double xPosition; 
        double yPosition; 
        double lastDirection; 
        int room;
        int id;

    public:
        Position(double xPosition, double yPosition, int room); 

        void updateXPosition(double updateX); 
        void updateYPosition(double updateY); 
        void changeXPosition(double changeX); 
        void changeYPosition(double changeY);
        void changeId(int newId);

        double getXPosition() const; 
        double getYPosition() const;
        int getId() const;
        void setLastDirection(double direction); 
        double getLastDirection(); 

        void updateRoom(int room); 
        int getRoom() const; 
};

#endif