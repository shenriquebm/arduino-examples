//
// Created by shenrique on 10/4/22.
//

#ifndef WIFI_CONNECT_CUSTOMEFFECTS_H
#define WIFI_CONNECT_CUSTOMEFFECTS_H

#include <array>

const std::array<std::array<uint8_t, 8>, 8> spiral = {
         0,  1,  2,  3,  4,  5,  6,  7,
         0,  1,  2,  3,  4,  5,  6,  7,
        23, 23, 24, 25, 26, 27,  8,  8,
        22, 22, 24, 25, 26, 27,  9,  9,
        21, 21, 31, 30, 28, 28, 10, 10,
        20, 20, 31, 30, 29, 29, 11, 11,
        19, 18, 17, 16, 15, 14, 12, 12,
        19, 18, 17, 16, 15, 14, 13, 13
};
const int spiralFrames = 32;
const int spiralAppearDelay = 2;

const std::array<std::array<uint8_t, 8>, 8> rain = {
        1, 15, 15,  1, 15, 15,  1, 15,
       14,  2, 14, 14,  2, 14, 14,  2,
       13,  3, 13, 13,  3, 13, 13,  3,
       12, 12,  4, 12, 12,  4, 12, 12,
        5, 11, 11,  5, 11, 11,  5, 11,
        6, 10, 10,  6, 10, 10,  6, 10,
        9,  7,  9,  9,  7,  9,  9,  7,
        8,  8,  8,  8,  8,  8,  8,  8
};
const int rainFrames = 32;
const int rainAppearDelay = 16;

const std::array<std::array<uint8_t, 8>, 8> rising = {
        8,  8,  8,  8,  8,  8,  8,  8,
        7,  7,  7,  7,  7,  7,  7,  7,
        6,  6,  6,  6,  6,  6,  6,  6,
        5,  5,  5,  5,  5,  5,  5,  5,
        4,  4,  4,  4,  4,  4,  4,  4,
        3,  3,  3,  3,  3,  3,  3,  3,
        2,  2,  2,  2,  2,  2,  2,  2,
        1,  1,  1,  1,  1,  1,  1,  1
};

const int risingFrames = 9;
const int risingAppearDelay = 0;

const std::array<std::array<uint8_t, 8>, 8> spiral2 = {
         1,  2,  3,  4,  5,  6,  7,  8,
        28, 29, 30, 31, 32, 33, 34,  9,
        27, 48, 49, 50, 51, 52, 35, 10,
        26, 47, 60, 61, 62, 53, 36, 11,
        25, 46, 59, 64, 63, 54, 37, 12,
        24, 45, 58, 57, 56, 55, 38, 13,
        23, 44, 43, 42, 41, 40, 39, 14,
        22, 21, 20, 19, 18, 17, 16, 15,
};
const int spiral2Frames = 65;
const int spiral2AppearDelay = 4;

#endif //WIFI_CONNECT_CUSTOMEFFECTS_H
