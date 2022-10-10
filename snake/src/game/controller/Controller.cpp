//
// Created by shenrique on 09/10/22.
//

#include "Controller.h"

int8_t AnalogJoystickController::x() {
    auto v = map(analogRead(xPin), JOYSTICK_MIN, JOYSTICK_MAX, 0, 100);
    if (v > 80) {
        return 1;
    } else if (v < 20) {
        return -1;
    }
    return 0;
}

int8_t AnalogJoystickController::y() {
    auto v = map(analogRead(yPin), JOYSTICK_MIN, JOYSTICK_MAX, 0, 100);
    if (v > 80) {
        return 1;
    } else if (v < 20) {
        return -1;
    }
    return 0;
}

bool AnalogJoystickController::btn() {
    return digitalRead(btnPin);
}

AnalogJoystickController::AnalogJoystickController(uint8_t xPin, uint8_t yPin, uint8_t btnPin) : xPin(xPin), yPin(yPin),
                                                                                                 btnPin(btnPin) {
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(btnPin, INPUT);
}

Controller::~Controller() = default;
