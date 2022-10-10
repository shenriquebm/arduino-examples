//
// Created by shenrique on 09/10/22.
//

#ifndef SNAKE_SCREEN_H
#define SNAKE_SCREEN_H
#include <Arduino.h>
#include <Wire.h>
#include <array>
#include <vector>
#include "Adafruit_SSD1306.h"


class Screen {
private:
    int16_t width = 128;
    int16_t height = 64;
    uint8_t scale = 1;
    Adafruit_SSD1306 screen;
    std::array<byte, 2> i2cScreenWire;

public:
    explicit Screen(const std::array<byte, 2> &i2CScreenWire, int16_t width=128, int16_t height=64, uint8_t scale=2);
    void drawBorders();
    void clear();
    void show();
    void addAndDeletePixels(std::array<uint8_t , 2> pixelToWrite, std::array<uint8_t, 2> pixelToDelete);
    void addPixels(const std::vector<std::array<uint8_t , 2>>& pixelToWrite);

    int16_t getWidth() const;

    int16_t getHeight() const;

    void setScale(uint8_t scale);

    void writeText(std::string text, uint8_t size, std::array<uint8_t, 2> pos,  bool selected = false);
    void drawTitleDetails();
};


#endif //SNAKE_SCREEN_H
