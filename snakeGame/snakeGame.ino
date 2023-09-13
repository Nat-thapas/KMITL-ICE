// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "mod.h"

#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP 4

int joyStickXmin = 0;
int joyStickXmid = 503;
int joyStickXmax = 1005;
int joyStickYmin = 0;
int joyStickYmid = 505;
int joyStickYmax = 1002;

// CS_PIN 5 (Must be PWN capable)
// DIN_PIN 11 (Does not seem to be configurable)
// CLK_PIN 13 (Does not seem to be configurable)

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 10

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_MAX72XX myDisplay = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int buttons[4] = {2, 3, 4, 5};
int joyStickX = A0;
int joyStickY = A1;

int getDir(float xval, float yval) {
    // Serial.print(xval);
    // Serial.print(" ");
    // Serial.println(yval);
    if (abs(xval) <= 0.25f && abs(yval) <= 0.25f) {
        return 0;
    }
    if (abs(xval) > abs(yval)) {
        if (xval > 0) {
            return RIGHT;
        } else {
            return LEFT;
        }
    } else {
        if (yval > 0) {
            return UP;
        } else {
            return DOWN;
        }
    }
}

float getNormalizedInput(int analogInput, char axis) {
    switch (axis) {
        case 'x':
            if (analogInput == joyStickXmid) {
                return 0.f;
            } else if (analogInput > joyStickXmid) {
                return (float)(analogInput - joyStickXmid) / (float)(joyStickXmax - joyStickXmid);
            } else {
                // Serial.print(analogInput - joyStickXmin);
                // Serial.print(" ");
                // Serial.println(joyStickXmid - joyStickXmin);
                return ((float)(analogInput - joyStickXmin) / (float)(joyStickXmid - joyStickXmin)) - 1.f;
            }
            break;
        case 'y':
            if (analogInput == joyStickYmid) {
                return 0.f;
            } else if (analogInput > joyStickYmid) {
                return (float)(analogInput - joyStickYmid) / (float)(joyStickYmax - joyStickYmid);
            } else {
                return ((float)(analogInput - joyStickYmin) / (float)(joyStickYmid - joyStickYmin)) - 1.f;
            }
            break;
        default:
            return 0.f;
    }
}

bool findInArray(int a, int b, int *arrA, int *arrB, int size) {
    for (int i=0; i<size; i++) {
        if (a == arrA[i] && b == arrB[i]) {
            return true;
        }
    }
    return false;
}

void setup() {
    Serial.begin(9600);
    for (int i=0; i<4; i++) {
        pinMode(buttons[i], INPUT_PULLUP);
    }
    pinMode(joyStickX, INPUT);
    pinMode(joyStickY, INPUT);
    pinMode(7, INPUT);
    myDisplay.begin();
    myDisplay.clear();
    myDisplay.setPoint(5, 4, true);
}

int x = 2;
int y = 3;
int dir = RIGHT;
int upperBound = 1;

int appleX = 5;
int appleY = 4;

int size = 1;
int oldSize = 1;

int loopCount = 0;

int snakeX[64] = {};
int snakeY[64] = {};

bool buttonStates[4] = {true, true, true, true};
bool lastButtonStates[4] = {true, true, true, true};

void loop() {
    for (int i=0; i<4; i++) {
        buttonStates[i] = digitalRead(buttons[i]);
    }
    float joyStickXval = getNormalizedInput(analogRead(joyStickX), 'x');
    float joyStickYval = getNormalizedInput(analogRead(joyStickY), 'y');
    for (int i=0; i<4; i++) {
        if (buttonStates[i] < lastButtonStates[i]) {
            switch(i) {
                case 0:
                    dir = UP;
                    break;
                case 1:
                    dir = RIGHT;
                    break;
                case 2:
                    dir = DOWN;
                    break;
                case 3:
                    dir = LEFT;
                    break;
            }
        }
    }
    int newJoyStickDir = getDir(joyStickXval, joyStickYval);
    if (newJoyStickDir > 0) {
        dir = newJoyStickDir;
    }
    if (loopCount % 6 == 0) {
        switch(dir) {
            case RIGHT:
                x++;
                x = mod(x, 8);
                break;
            case DOWN:
                y--;
                y = mod(y, 8);
                break;
            case LEFT:
                x--;
                x = mod(x, 8);
                break;
            case UP:
                y++;
                y = mod(y, 8);
                break;
        }
        // Serial.println("xxxxxx");
        // for (int i=0; i<size; i++) {
        //     Serial.print(snakeX[i]);
        //     Serial.println(snakeY[i]);
        // }
        // Serial.println("-------");
        // Serial.print(x);
        // Serial.println(y);
        if (findInArray(x, y, snakeX, snakeY, size)) {
            pinMode(7, OUTPUT);
            digitalWrite(7, LOW);
            delay(1000);
        }
        myDisplay.setPoint(x, y, true);
        snakeX[oldSize] = x;
        snakeY[oldSize] = y;
        if (x == appleX && y == appleY) {
            size++;
            appleX = random(0, 7);
            appleY = random(0, 7);
            while (findInArray(appleX, appleY, snakeX, snakeY, 8)) {
                appleX = random(0, 7);
                appleY = random(0, 7);
            }
            myDisplay.setPoint(appleX, appleY, true);
        }
        if (size == oldSize) {
            myDisplay.setPoint(snakeX[0], snakeY[0], false);
            for (int i=0; i<size; i++) {
                snakeX[i] = snakeX[i+1];
                snakeY[i] = snakeY[i+1];
            }
        }
        oldSize = size;
    }
    loopCount++;
    for (int i=0; i<4; i++) {
        lastButtonStates[i] = buttonStates[i];
    }
    delay(50);
}
