//
// Created by shenrique on 09/10/22.
//

#ifndef SNAKE_MENU_H
#define SNAKE_MENU_H


#include <unordered_map>
#include "game/screen/Screen.h"
#include "game/controller/Controller.h"

class Menu {
private:
    Screen screen;
    Controller* controller;
    int8_t mapSizeOpt = 0;
    int8_t difficulty = 9;
    int8_t selectedOpt = 0;
    unsigned long lastTick = 0;
    bool start = false;

public:
    Menu(const Screen &screen, Controller *controller);

    void drawOptions();
    std::pair<bool, std::array<int8_t, 2>> show();
    void drawTitle();
    std::string getMapSizeStr() const;
    void processInput();
};


#endif //SNAKE_MENU_H
