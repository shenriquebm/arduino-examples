//
// Created by shenrique on 10/3/22.
//

#include "Led88Writer.h"
#include "strMap.h"
#include "customEffects.h"


Led88Writer::Led88Writer(const std::array<byte, 8> &pinsX, const std::array<byte, 8> &pinsY) : pinsX(pinsX),
                                                                                               pinsY(pinsY) {}

void Led88Writer::show() {
    auto _buf = this->buf;
    if (ledEvent.second > millis()) {
        _buf = ledEvent.first;
    }
    for (byte i = 0; i < 8; i++) {
        digitalWrite(pinsX[i], HIGH);
        for (byte j = 0; j < 8; j++) {
            digitalWrite(pinsY[j], !_buf[j][i]);
        }
        delayMicroseconds(20);
        for (byte j = 0; j < 8; j++) {
            digitalWrite(pinsY[j], HIGH);
        }
        digitalWrite(pinsX[i], LOW);
    }
}

void Led88Writer::show(const std::string & str) {
    if (str.empty() || !started) {
        text = str + " ";
        started = true;
        buf = strMap[text[0]];
    }
    switch (mode) {
        case BLINK:
            blinkWrite();
            break;
        case SLIDE:
            slideWrite();
            break;
        case SWIPE:
            swipeWrite();
            break;
        case SHRINK:
            shrinkWrite();
            break;
        default:
            customWrite();
            break;
    }
}

int Led88Writer::getTransitionTime() const {
    switch (mode) {
        case SLIDE:
        case SWIPE:
            return transitionTime / 8;
        case SHRINK:
            return transitionTime / 4;
        case BLINK:
        default:
            return transitionTime;
    }
}

bool Led88Writer::waitUnblocking(const std::string& eventName, unsigned long ms) {
    eventStepMap.emplace(eventName, millis() + ms);
    return eventStepMap[eventName] < millis();
}

void Led88Writer::releaseWait(const std::string& eventName) {
    eventStepMap.erase(eventName);
}

bool Led88Writer::startLedEvent(std::array<std::array<bool, 8>, 8> bufEvent, int duration) {
    if (ledEvent.second == 0) {
        ledEvent = std::make_pair(bufEvent, millis() + duration);
    }
    return ledEvent.second < millis();
}

void Led88Writer::releaseLedEvent() {
    ledEvent = {};
}

void Led88Writer::blinkWrite() {
    int curr = getIter("curr");
    {
        buf = strMap[text[curr]];
        this->ledWrite();
        if (!waitUnblocking("blinkWrite", getTransitionTime())) return;

        if (!startLedEvent(strMap[' '], 100)) return;
        releaseLedEvent();
        releaseWait("blinkWrite");
    } if (stepIter("curr", text.size())) return;
}

void Led88Writer::ledWrite() {
    if (started) {
        this->show();
    }
}

void Led88Writer::stop() {
    this->started = false;
    this->iterMap.clear();
    this->eventStepMap.clear();
    this->ledEvent = {};
    text = "";
}

void Led88Writer::slideWrite() {
    if (text.length() == 1) {
        text += text;
    } else if (text[text.length() - 1] != ' ') {
        text += " ";
    }

    // for strIdx = 0; strIdx < text.size(); strIdx++
    int strIdx = getIter("strIdx");
    {
        auto current = strMap[text[strIdx]];
        std::array<std::array<bool, 8>, 8> next = {};
        if (strIdx + 1 == text.size()) {
            next = strMap[text[0]];
        } else {
            next = strMap[text[strIdx + 1]];
        }

        int iterN = getIter("iterN");
        {
            for (int row = 0; row < current.size(); row++) {
                for (int col = 0; col < current[row].size(); col++) {
                    int colIdx = col + iterN;
                    if (colIdx >= current[row].size()) {
                        buf[row][col] = next[row][colIdx - 8];
                    } else {
                        buf[row][col] = current[row][colIdx];
                    }
                }
            }
            this->ledWrite();
            if (!waitUnblocking("slideWrite", getTransitionTime())) return;
            releaseWait("slideWrite");

        } if (stepIter("iterN", current[0].size())) return;
    } if (stepIter("strIdx", text.size())) return;
}

void Led88Writer::swipeWrite() {
    if (text.length() == 1) {
        text += text;
    }

    // for strIdx = 0; strIdx < text.size(); strIdx++
    int strIdx = getIter("strIdx");
    {
        auto current = strMap[text[strIdx]];
        std::array<std::array<bool, 8>, 8> next = {};
        if (strIdx + 1 == text.size()) {
            next = strMap[text[0]];
        } else {
            next = strMap[text[strIdx + 1]];
        }

        int iterN = getIter("iterN");
        {
            for (int row = 0; row < current.size(); row++) {
                for (int col = 0; col < current[row].size(); col++) {
                    if (row < iterN) {
                        buf[row][col] = next[row][col];
                    } else if (row == iterN) {
                        buf[row][col] = true;
                    } else {
                        buf[row][col] = current[row][col];
                    }
                }
            }
            this->ledWrite();
            if (!waitUnblocking("slideWrite", getTransitionTime())) return;
            releaseWait("slideWrite");

        } if (stepIter("iterN", current[0].size() + 3)) return;
        Serial.println("here");

    } if (stepIter("strIdx", text.size())) return;
}

void Led88Writer::shrinkWrite() {
    if (text.length() == 1) {
        text += text;
    }

    // for strIdx = 0; strIdx < text.size(); strIdx++
    int strIdx = getIter("strIdx");
    {
        auto current = strMap[text[strIdx]];
        std::array<std::array<bool, 8>, 8> next = {};
        if (strIdx + 1 == text.size()) {
            next = strMap[text[0]];
        } else {
            next = strMap[text[strIdx + 1]];
        }

        int iterN = getIter("iterN");
        {
            for (int row = 0; row < current.size(); row++) {
                for (int col = 0; col < current[row].size(); col++) {
                    if (((row - iterN == 0 || row + iterN == current.size() - 1) && col >= iterN &&
                         col < current[row].size() - iterN) ||
                        ((col - iterN == 0 || col + iterN == current[row].size() - 1) && row >= iterN &&
                         row < current.size() - iterN)) {
                        buf[row][col] = true;
                    } else if (row - iterN < 0 || row + iterN >= current.size() || col - iterN < 0 ||
                               col + iterN >= current[row].size()) {
                        buf[row][col] = next[row][col];
                    }
                }
            }
            this->ledWrite();
            if (!waitUnblocking("shrinkWrite", getTransitionTime())) return;
            releaseWait("shrinkWrite");

        }
        if (stepIter("iterN", current[0].size() / 2 + 2)) return;
    }
    if (stepIter("strIdx", text.size())) return;
}

void Led88Writer::customWrite() {
    switch (mode) {
        case SPIRAL:
            customWrite(spiral, spiralFrames, spiralAppearDelay);
            break;
        case RAIN:
            customWrite(rain, rainFrames, rainAppearDelay);
            break;
        case RISE:
            customWrite(rising, risingFrames, risingAppearDelay);
            break;
        case SPIRAL2:
            customWrite(spiral2, spiral2Frames, spiral2AppearDelay);
            break;
    }
}

void Led88Writer::customWrite(std::array<std::array<uint8_t, 8>, 8> effectMap, uint8_t frames, uint8_t appearDelay) {
    if (text.length() == 1) {
        text += text;
    }

    // for strIdx = 0; strIdx < text.size(); strIdx++
    int strIdx = getIter("strIdx");
    {
        auto current = strMap[text[strIdx]];
        std::array<std::array<bool, 8>, 8> next = {};
        if (strIdx + 1 == text.size()) {
            next = strMap[text[0]];
        } else {
            next = strMap[text[strIdx + 1]];
        }

        int iterN = getIter("iterN");
        {
            for (int row = 0; row < current.size(); row++) {
                for (int col = 0; col < current[row].size(); col++) {
                    if (effectMap[row][col] == iterN) {
                        buf[row][col] = true;
                    }
                    if (effectMap[row][col] == iterN - appearDelay) {
                        buf[row][col] = next[row][col];
                    }
                }
            }
            this->ledWrite();
            if (!waitUnblocking("slideWrite", getTransitionTime() / (frames + appearDelay))) return;
            releaseWait("slideWrite");

        }
        if (stepIter("iterN", frames + (appearDelay * 2))) return;
    }
    if (stepIter("strIdx", text.size())) return;
}

int Led88Writer::getIter(const std::string& iterName) {
    iterMap.emplace(iterName, 0);
    return iterMap[iterName];
}

bool Led88Writer::stepIter(const std::string& iterName, int to) {
    iterMap[iterName] += 1;
    if (iterMap[iterName] >= to)  {
        iterMap[iterName] = 0;
        return false;
    }
    return true;
}
