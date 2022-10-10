<!-- TOC -->
* [8x8 LED 788BS](#8x8-led-788bs)
  * [Basic example](#basic-example)
  * [Additional Examples](#additional-examples)
  * [Adding new transition animations](#adding-new-transition-animations)
<!-- TOC -->

# 8x8 LED 788BS

Hardware/software used
```
Board: Adafruit ESP32 feather

❯ pio --version
PlatformIO Core, version 6.1.4

788BS 8x8 LED matrix
```

---
## Basic example

```c
#include <Arduino.h>
#include <memory>
#include "Led88Writer.h"

std::array<byte, 8> pinsX = {19, 18, 16, 5, 15, 17, 33, 32};
std::array<byte, 8> pinsY = {27, 26, 25, 22, 21, 14, 12, 13};
WriteMode mode = BLINK; // See include/LedWriter.h#9:19 for supported types
std::unique_ptr<Led88Writer> ledWriter(new Led88Writer(pinsX, pinsY));

void setup() {
    Serial.begin(9600);
    for (auto i = 0; i < 8; i++) {
        pinMode(pinsX[i], OUTPUT);
        pinMode(pinsY[i], OUTPUT);
    }
    ledWriter->mode = mode;
    ledWriter->transitionTime = 1000;
}

void loop() {
    ledWriter->show("Led 8x8 Example");
}
```

Step by step

```c
std::array<byte, 8> pinsX = {19, 18, 16, 5, 15, 17, 33, 32};
std::array<byte, 8> pinsY = {27, 26, 25, 22, 21, 14, 12, 13};
```
Save all the GPIO used to connect the LED, pinsX has the columns and pinsY has the rows.

```c
WriteMode mode = BLINK; // See include/LedWriter.h#9:19 for supported types
```
There are some transition animations available, all the modes should be inside `include/LedWriter.h` at lines 9 to 19.

```c
std::unique_ptr<Led88Writer> ledWriter(new Led88Writer(pinsX, pinsY));
```
Initializes the ledWriter.

```c
for (auto i = 0; i < 8; i++) {
    pinMode(pinsX[i], OUTPUT);
    pinMode(pinsY[i], OUTPUT);
}
```
Sets all the pins connected to the LED to output mode.

```c
ledWriter->mode = mode;
ledWriter->transitionTime = 1000;
```
Sets the ledWriter transition animation to the mode defined earlier. Set the transition time (i.e. time it takes to
finish animation from one character to another) in ms.

```c
void loop() {
    ledWriter->show("Led 8x8 Example");
}
```
Our loop calls show, which will then show the text.

## Additional Examples

```c
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
```
Adds a push button and a potentiometer to the setup. When pressing the button, it will cycle through all the available
animations.
```c
if (digitalRead(btnPin) == HIGH) {
    mode++;
    mode %= WriteMode::SIZE;
    ledWriter->stop();
    ledWriter->mode = (WriteMode)mode;
    while(digitalRead(btnPin) == HIGH);
}
```
calling `ledWriter->stop()` resets the state. Before changing animations, or the text displayed, it is necessary to call
`stop()`.
It is not necessary to call stop to change the transition speed or

## Adding new transition animations

Inside `include/customEffects.h` there is a place saving some of the transition animations.
It is necessary 3 additional inputs:
* An 8x8 frame matrix
* An int telling the total number of frames
* The appear delay, from when the frame is rendered, to when it'll contain the value of the next character. Setting this
to 0, means that the corresponding pixel for the character will show immediately. Setting to any positive value will
first show a red pixel on that location for `N` frames, where `N` is the delay.
```c
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
```

This is an example of an spiral effect. It will first render the first row (left to right), then the last column (top to
bottom), and so on. The way to determine the transition is by looking at the numbers inside the frame matrix.

```
At frame 0, pixels (0, 0) and (1, 0) light up, since they have 0 in the frame matrix;

At frame 1, pixels (0, 1) and (1, 1) light up, since they have 1 in the frame matrix;

At frame 2, pixels (0, 2) and (1, 2) light up, since they have 2 in the frame matrix;

and so on.
```

There's a total of 32 frames, and for animation, there's a frame delay of 2.

After creating a new animation, add it to the `WriteMode` enum in Led88Writer.h 
```c
enum WriteMode{
    BLINK,
    SLIDE,
    SWIPE,
    SHRINK,
    SPIRAL,
    SPIRAL2,
    RAIN,
    RISE,
    // <- Add it before SIZE
    SIZE // SIZE is not an available animation, is there just for easily cycling through animations
};
```
Finally, use the enum created and the variables inside `Led88Writer.cpp` at the function `customWrite()`:

```c
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
        // <--- Add any new animations here
        // case ANIM:
        //     customWrite(anim, animFrames, animAppearDelay);
        //     break;
    }
}
```
Then, animation is available to be used.

