//
// Created by shenrique on 09/10/22.
//

#include "Screen.h"

Screen::Screen(const std::array<byte, 2> &i2CScreenWire, int16_t width, int16_t height, uint8_t scale) : width(width),
                                                                                                         height(height),
                                                                                                         i2cScreenWire(i2CScreenWire),
                                                                                                         scale(scale) {
    Wire.begin(this->i2cScreenWire[0], this->i2cScreenWire[1]);
    screen = Adafruit_SSD1306(width, height, &Wire);
    if (!screen.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Failed to initialize screen"));
        for (;;);
    }
}

void Screen::drawBorders() {
    for (uint8_t i = 0; i < scale; i++) {
        screen.drawFastHLine(0, i, width, SSD1306_WHITE);
        screen.drawFastHLine(0, height - 1 - i, width, SSD1306_WHITE);
        screen.drawFastVLine(i, 0, height, SSD1306_WHITE);
        screen.drawFastVLine(width - 1 - i, 0, height, SSD1306_WHITE);
    }
}

void Screen::clear() {
    screen.clearDisplay();
}

void Screen::show() {
    screen.display();
}

void Screen::addAndDeletePixels(std::array<uint8_t , 2> pixelToWrite, std::array<uint8_t, 2> pixelToDelete) {
    if (pixelToWrite == pixelToDelete) {
        return;
    }
    for (uint8_t i = 0; i < scale; i++) {
        for (uint8_t j = 0; j < scale; j++) {
            screen.drawPixel((pixelToWrite[0] * scale) + i, (pixelToWrite[1] * scale) + j, SSD1306_WHITE);
            screen.drawPixel((pixelToDelete[0] * scale) + i, (pixelToDelete[1] * scale) + j, SSD1306_BLACK);
        }
    }
}

void Screen::addPixels(const std::vector<std::array<uint8_t, 2>>& pixelToWrite) {
    for (auto pixel: pixelToWrite) {
        for (uint8_t i = 0; i < scale; i++) {
            for (uint8_t j = 0; j < scale; j++) {
                screen.drawPixel((pixel[0] * scale) + i, (pixel[1] * scale) + j, SSD1306_WHITE);
            }
        }
    }
}

int16_t Screen::getWidth() const {
    return width / scale;
}

int16_t Screen::getHeight() const {
    return height / scale;
}

void Screen::writeText(std::string text, uint8_t size, std::array<uint8_t, 2> pos, bool selected) {
    screen.setTextSize(size);
    if (selected) {
        screen.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        screen.setTextColor(SSD1306_WHITE);
    }
    screen.setCursor(pos[0], pos[1]);
    screen.cp437();
    screen.write(text.c_str());
}

void Screen::setScale(uint8_t scale) {
    Screen::scale = scale;
}

void Screen::drawTitleDetails() {
    screen.drawFastHLine(0, 10, 30, SSD1306_WHITE);
    screen.drawFastHLine(97, 10, 30, SSD1306_WHITE);
}


