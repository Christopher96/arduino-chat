#include "Joystick.h"

// Constructor
Joystick::Joystick() {
    Serial.begin(115200);
}

bool Joystick::isIdle() {
    return (getX() > 450 && getX() < 550) && (getY() > 450 && getY() < 550);
}
bool Joystick::isUp() {
    return (getX() > JS_MAX);
}

bool Joystick::isDown() {
    return (getX() < JS_MIN);
}

bool Joystick::isRight() {
    return (getY() > JS_MAX);
}

bool Joystick::isLeft() {
    return (getY() < JS_MIN);
}

int Joystick::getX() {
    return analogRead(JS_X_PIN);
}

int Joystick::getY() {
    return analogRead(JS_Y_PIN);
}
