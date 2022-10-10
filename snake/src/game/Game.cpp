//
// Created by shenrique on 09/10/22.
//

#include "Game.h"

Game::Game(std::array<byte, 2> i2cScreenWires, Controller *controller, uint8_t difficulty) :
        screen(Screen(i2cScreenWires)), controller(controller), difficulty(difficulty), menu(Menu(screen, controller)) {
}

void Game::endGame() {
    started = false;
    resetGame();
}

void Game::resetGame() {
    while (!player.empty()) {
        player.pop();
    }
    player.push({1, 1});
    player.push({2, 1});
    player.push({3, 1});
    player.push({4, 1});
    player.push({5, 1});

    playerPos.clear();
    playerPos.insert({1, 1});
    playerPos.insert({2, 1});
    playerPos.insert({3, 1});
    playerPos.insert({4, 1});
    playerPos.insert({5, 1});

    screen.clear();
    screen.drawBorders();
    screen.addPixels({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});
    playerHead = {5, 1};
    hasEaten = false;

    initSpawnLocation();
    spawnLocations.erase({1, 1});
    spawnLocations.erase({2, 1});
    spawnLocations.erase({3, 1});
    spawnLocations.erase({4, 1});
    spawnLocations.erase({5, 1});

    auto ar0 = micros();
    randomSeed(ar0);
    spawnFood();
    difficulty = min(max((int)difficulty, 1), 9);
    gameMs = tickMsDifficulty[difficulty];
    lastTick = 0;
    direction = RIGHT;
    hasEaten = false;
}

void Game::loop() {
    if (started) {
        tick();
        screen.show();
    } else {
        showGameMenu();
    }
}

void Game::tick() {
    processControllerInput();
    if (millis() - lastTick > gameMs) {
        lastTick = millis();
        moveAndDrawPlayer();
        if (verifyCollision()) {
            delay(1000);
            endGame();
        }
        if (verifyFoodCollected()) {
            hasEaten = true;
            spawnFood();
        }
    }
}

void Game::processControllerInput() {
    auto x = controller->x();
    auto y = controller->y();
    if (nextDirection == IDLE) {
        if (x) {
            if (direction != LEFT && direction != RIGHT) {
                if (x > 0) {
                    nextDirection = RIGHT;
                } else {
                    nextDirection = LEFT;
                }
            }
        } else if (y) {
            if (direction != UP && direction != DOWN) {
                if (y > 0) {
                    nextDirection = UP;
                } else {
                    nextDirection = DOWN;
                }
            }
        }
    }
}

void Game::moveAndDrawPlayer() {
    std::array<byte, 2> deletedPos = player.front();
    if (!hasEaten) {
        player.pop();
        playerPos.erase(deletedPos);
        spawnLocations.insert(deletedPos);
    }
    if (nextDirection != IDLE) {
        direction = nextDirection;
        nextDirection = IDLE;
    }
    switch (direction) {
        case UP:
            playerHead[1]--;
            break;
        case DOWN:
            playerHead[1]++;
            break;
        case LEFT:
            playerHead[0]--;
            break;
        case RIGHT:
        default:
            playerHead[0]++;
    }
    player.push(playerHead);
    playerPos.insert(playerHead);
    spawnLocations.erase(playerHead);
    if (!hasEaten) {
        screen.addAndDeletePixels(playerHead, deletedPos);
    } else {
        screen.addPixels({playerHead});
    }
    hasEaten = false;
}

bool Game::verifyCollision() {
    auto pX = playerHead[0];
    auto pY = playerHead[1];

    if (pX == 0 || pY == 0 || pX == screen.getWidth() - 1 || pY == screen.getHeight() - 1) {
        return true;
    }
    if (playerPos.size() != player.size()) {
        return true;
    }
    return false;
}

bool Game::verifyFoodCollected() {
    return playerHead == food;
}

void Game::spawnFood() {
    auto r = random(spawnLocations.size());
    auto it = spawnLocations.begin();
    std::advance(it, r);
    auto p = *it;
    food = p;
    screen.addPixels({food});
}

void Game::initSpawnLocation() {
    spawnLocations.clear();
    for (byte x = 1; x < screen.getWidth() - 1; x++) {
        for (byte y = 1; y < screen.getHeight() - 1; y++) {
            spawnLocations.insert({x, y});
        }
    }
}

void Game::showGameMenu() {
    auto res = menu.show();
    started = res.first;
    difficulty = res.second[0];
    switch (res.second[1]) {
        case 0:
            screen.setScale(4);
            break;
        case 1:
            screen.setScale(2);
            break;
        case 2:
            screen.setScale(1);
    }
    if (started) {
        resetGame();
    }
}

