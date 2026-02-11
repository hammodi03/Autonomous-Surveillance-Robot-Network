#ifndef PERSON_H
#define PERSON_H

#include <Arduino.h>

class Person {
private:
    int id;
    double x;
    double y;
    int roomNbr;

public:
    Person();

    // Getters
    int getId() const;
    double getX() const;
    double getY() const;
    int getRoom() const;

    // Setters
    void setId(int val);
    void setX(double val);
    void setY(double val);
    void setRoom(int val);
};

#endif
