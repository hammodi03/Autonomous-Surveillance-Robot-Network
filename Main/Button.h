#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
    private:
        int buttonPin;
        int buttonValue;

    public:
        Button(int pin); 
        void init(); 
        int getButtonValue();
};
#endif 