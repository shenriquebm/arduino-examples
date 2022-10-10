//
// Created by shenrique on 09/10/22.
//

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H


#include <queue>
#include <unordered_set>
#include "game/screen/Screen.h"
#include "game/controller/Controller.h"
#include "Menu.h"

namespace std
{
    template<typename T, size_t N>
    struct hash<array<T, N> >
    {
        typedef array<T, N> argument_type;
        typedef size_t result_type;

        result_type operator()(const argument_type& a) const
        {
            hash<T> hasher;
            result_type h = 0;
            for (result_type i = 0; i < N; ++i)
            {
                h = h * 31 + hasher(a[i]);
            }
            return h;
        }
    };
}


class Game {
private:
    std::vector<long> tickMsDifficulty = {9999999, 400, 350, 300, 250, 200, 150, 100, 50, 0};
    enum MoveDirection {
        UP,
        LEFT,
        DOWN,
        RIGHT,
        IDLE
    };
    std::unordered_set<std::array<byte, 2>> spawnLocations;
    std::array<byte, 2> food = {0, 0};
    bool hasEaten = false;
    MoveDirection direction = RIGHT;
    MoveDirection nextDirection = IDLE;
    std::queue<std::array<byte, 2>> player;
    std::unordered_set<std::array<byte, 2>> playerPos;
    std::array<byte, 2> playerHead = {0, 0};
    Screen screen;
    Controller* controller;
    uint8_t difficulty;
    Menu menu;
    unsigned long lastTick;
    long gameMs;
    bool started;
    void tick();
    void processControllerInput();
    void moveAndDrawPlayer();
    bool verifyCollision();
    bool verifyFoodCollected();
    void spawnFood();
    void initSpawnLocation();
    void resetGame();
    void endGame();
    void showGameMenu();
public:
    explicit Game(std::array<byte, 2> i2cScreenWires, Controller* controller, uint8_t difficulty = 9);
    void loop();
};


#endif //SNAKE_GAME_H
