#include <Arduino.h>
#include <memory>
#include "Led88Writer.h"

std::array<byte, 8> pinsX = {19, 18, 16, 5, 15, 17, 33, 32};
std::array<byte, 8> pinsY = {27, 26, 25, 22, 21, 14, 12, 13};
byte btnPin = 23;
byte potentiometerPin = 4;
auto mode = 0;
std::unique_ptr<Led88Writer> ledWriter(new Led88Writer(pinsX, pinsY));

void setup() {
    Serial.begin(9600);
    for (auto i = 0; i < 8; i++) {
        pinMode(pinsX[i], OUTPUT);
        pinMode(pinsY[i], OUTPUT);
    }
    pinMode(btnPin, INPUT);
    pinMode(potentiometerPin, INPUT);
    ledWriter->mode = (WriteMode)mode;
}

void loop() {
    ledWriter->show("Led 8x8 Example");

    if (digitalRead(btnPin) == HIGH) {
        mode++;
        mode %= WriteMode::SIZE;
        ledWriter->stop();
        ledWriter->mode = (WriteMode)mode;
        while(digitalRead(btnPin) == HIGH);
    }
    ledWriter->transitionTime = map(analogRead(potentiometerPin), 0, 4095, 250, 2500);
}