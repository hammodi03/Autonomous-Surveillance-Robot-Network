#include "DetectionController.h"

DetectionController::DetectionController(int frontPin, int leftPin, int rightPin, int leftFBPin, int rightFBPin):
    frontIR(frontPin), leftIR(leftPin), rightIR(rightPin), leftFrontButton(leftFBPin), rightFrontButtion(rightFBPin) {}


void DetectionController::initDetection()
{
    initIRSensors();
    //Serial.println("Innan button"); 
    initButtons(); 
    //Serial.println("Efter button");
}

void DetectionController::initIRSensors()
{
    frontIR.setupIrSensor(); 
    leftIR.setupIrSensor(); 
    rightIR.setupIrSensor(); 
}

void DetectionController::updateIRSensors()
{
    frontIR.update(); 
    if(0 == frontIR.getIrValue())
    {
        frontIRIndication = true; 
    }
    else
    {
        frontIRIndication = false; 
    }
    leftIR.update(); 
    if(0 == leftIR.getIrValue())
    {
        leftIRIndication = true; 
    } 
    else
    {
        leftIRIndication = false; 
    }
    rightIR.update(); 
    if(0 == rightIR.getIrValue())
    {
        rightIRIndication = true; 
    }
    else
    {
        rightIRIndication = false; 
    }
}

void DetectionController::initButtons()
{
    leftFrontButton.init(); 
    rightFrontButtion.init(); 
}

void DetectionController::updateButtonValues()
{
    bool lfbValue; 
    lfbValue = leftFrontButton.getButtonValue(); 
    leftFrontBLow = ! lfbValue; // If 0 (false) the button is pressed
    if(leftFrontBLow) 
    {
        secondsAfterStartLFBPushed = millis()/1000; 
    }
    bool rfbValue;
    rfbValue = rightFrontButtion.getButtonValue(); 
    rightFrontBLow = ! rfbValue; // If 0 (false) the button is pressed
    if(rightFrontBLow)
    {
        secondsAfterStartRFBPushed = millis()/1000; 
    }

    double secondsAfterStartNow = millis()/1000; 

    if(secondsAfterStartNow - secondsAfterStartLFBPushed <= 10 || secondsAfterStartNow - secondsAfterStartRFBPushed <= 10)
    {
        stoppedAfterButtonPusched = true; 
    }
    else
    {
        stoppedAfterButtonPusched = false; 
    }
}

bool DetectionController::checkStoppedAfterButtonPushed()
{
    return stoppedAfterButtonPusched; 
}

bool DetectionController::getFrontIRValue()
{
    return frontIRIndication; 
}

bool DetectionController::getLeftIRValue()
{
    return leftIRIndication; 
}

bool DetectionController::getRightIRValue()
{
    return rightIRIndication; 
}
