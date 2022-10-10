//
// Created by shenrique on 09/10/22.
//

#include "Menu.h"

std::pair<bool, std::array<int8_t, 2>> Menu::show() {
    screen.clear();
    screen.drawBorders();
    drawTitle();
    drawOptions();
    processInput();
    screen.show();
    std::pair<bool, std::array<int8_t, 2>> result = {start, {difficulty, mapSizeOpt}};
    start = false;
    return result;
}

void Menu::drawTitle() {
    screen.drawTitleDetails();
    screen.writeText("SNAKE", 2, {35, 5});
}

Menu::Menu(const Screen &screen, Controller *controller) : screen(screen), controller(controller) {}

void Menu::drawOptions() {
    screen.writeText("New game", 1, {10, 25}, selectedOpt == 0);
    std::string difficultyStr = "Difficulty: < " + std::to_string(difficulty) + " >";
    screen.writeText(difficultyStr, 1, {10, 37}, selectedOpt == 1);
    std::string mapSizeStr = "Map Size: < " + getMapSizeStr() + " >";
    screen.writeText(mapSizeStr, 1, {10, 49}, selectedOpt == 2);
}

std::string Menu::getMapSizeStr() const {
    switch (mapSizeOpt) {
        case 0:
            return "small";
        case 1:
            return " med ";
        case 2:
        default:
            return " big ";
    }
}

void Menu::processInput() {
    if (millis() - lastTick > 150) {
        lastTick = millis();
        auto x = controller->x();
        auto y = controller->y();
        // y is inverted
        if (y) {
            selectedOpt -= y;
            selectedOpt = max(min(selectedOpt, (int8_t)2), (int8_t)0);
        }
        if (x) {
            if (selectedOpt == 1) {
                difficulty += x;
                difficulty = min(max(difficulty, (int8_t)1), (int8_t)9);
            } else if (selectedOpt == 2) {
                mapSizeOpt += x;
                mapSizeOpt = max(min(mapSizeOpt, (int8_t)2), (int8_t)0);
            }
        }
    }
    if (controller->btn() && selectedOpt == 0) {
        start = true;
    }
}
