//
// Created by shenrique on 09/10/22.
//

#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

#include <Arduino.h>

#define JOYSTICK_MIN 0l
#define JOYSTICK_MAX 4096l

class Controller {
protected:
public:
    virtual int8_t x() = 0;
    virtual int8_t y() = 0;
    virtual bool btn() = 0;

    virtual ~Controller() = 0;
};

class AnalogJoystickController: public Controller {
private:
    uint8_t xPin;
    uint8_t yPin;
    uint8_t btnPin;
public:
    int8_t x() override;
    int8_t y() override;
    bool btn() override;

    AnalogJoystickController(uint8_t xPin, uint8_t yPin, uint8_t btnPin);
};


#endif //SNAKE_CONTROLLER_H
