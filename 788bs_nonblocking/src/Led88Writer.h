//
// Created by shenrique on 10/3/22.
//
#include "Arduino.h"
#include <unordered_map>

#ifndef LED88WRITER_H
#define LED88WRITER_H
enum WriteMode{
    BLINK,
    SLIDE,
    SWIPE,
    SHRINK,
    SPIRAL,
    SPIRAL2,
    RAIN,
    RISE,
    SIZE // SIZE is not an available animation, is there just for easily cycling through animations
};

class Led88Writer {
private:
    std::array<std::array<bool, 8>, 8> buf = {};
    unsigned long step = 0;
    unsigned long nextStep = 0;
    std::string text;
    std::array<byte, 8> pinsX;
    std::array<byte, 8> pinsY;
    std::unordered_map<std::string, int> iterMap = {};
    std::pair<std::array<std::array<bool, 8>, 8>, unsigned long> ledEvent = {};
    std::unordered_map<std::string, ulong> eventStepMap = {};
    boolean started = false;

    void show();
    void blinkWrite();
    void slideWrite();
    int getIter(const std::string&);
    void ledWrite();
    int getTransitionTime() const;
    bool stepIter(const std::string& iterName, int);
    bool waitUnblocking(const std::string& eventName, unsigned long ms);
    void releaseWait(const std::string& eventName);
    bool startLedEvent(std::array<std::array<bool, 8>, 8> bufEvent, int duration);
    void releaseLedEvent();
    void swipeWrite();
    void shrinkWrite();
    void customWrite();
    void customWrite(std::array<std::array<uint8_t, 8>, 8> effectMap, uint8_t frames, uint8_t);


public:
    WriteMode mode = BLINK;
    int transitionTime = 1000;
    Led88Writer(const std::array<byte, 8> &pinsX, const std::array<byte, 8> &pinsY);
    void show(const std::string &str);
    void stop();
};


#endif //LED88WRITER_H
