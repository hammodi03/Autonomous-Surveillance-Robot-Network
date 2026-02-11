#ifndef DETECTION_CONTROLLER_H
#define DETECTION_CONTROLLER_H
#include "IRSensor.h"
#include "Button.h"

class DetectionController 
{
    private:
        IrSensor frontIR;
        bool frontIRIndication = false; ; 
        IrSensor leftIR;
        bool leftIRIndication = false; 
        IrSensor rightIR; 
        bool rightIRIndication = false; 
        Button leftFrontButton;
        bool leftFrontBLow = false;
        Button rightFrontButtion;
        bool rightFrontBLow = false;
        bool stoppedAfterButtonPusched = false;
        double secondsAfterStartLFBPushed = 0; 
        double secondsAfterStartRFBPushed = 0; 

    public:
        DetectionController(int frontPin, int leftPin, int rightPin, int leftFBPin, int rightFBPin); 
        void initDetection(); 
        void initIRSensors(); 
        void initButtons(); 

        void updateButtonValues(); 
        bool checkStoppedAfterButtonPushed(); 

        void updateIRSensors(); 

        bool getFrontIRValue(); 
        bool getLeftIRValue(); 
        bool getRightIRValue(); 


};



#endif