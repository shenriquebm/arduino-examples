#include <Arduino.h>
#include "game/Game.h"

void appStart() {
    Serial.begin(9600);
    std::array<byte, 2> i2cScreenWires = {1, 2};
    AnalogJoystickController controller(6, 7, 5);
    Game game(i2cScreenWires, &controller);

    while (true) {
        game.loop();
    }
}

void setup() {
    appStart();
}

void loop() {}