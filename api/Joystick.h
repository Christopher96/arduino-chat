#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h> 
#include "constants.h"

// library interface description
class Joystick
{
    public:
        Joystick(); 
        bool isIdle();
        bool isUp();
        bool isDown();
        bool isLeft();
        bool isRight();
        int getX();
        int getY();
};


#endif
