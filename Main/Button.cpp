#include "Button.h"

Button::Button(int pin):buttonPin(pin){}

void Button::init()
{
    pinMode(buttonPin, INPUT_PULLUP);
}

int Button::getButtonValue()
{
    buttonValue = digitalRead(buttonPin); 
    return buttonValue;
}
