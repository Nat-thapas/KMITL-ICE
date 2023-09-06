// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP 4

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

void setup() {
    myDisplay.begin();
    myDisplay.clear();
}

int x = 0;
int y = 0;
int dir = RIGHT;
int upperBound = 1;
int lowerBound = 7;
int leftBound = 0;
int rightBound = 7;

bool mode = true;

void loop() {
    myDisplay.setPoint(x, y, mode);
    switch(dir) {
        case RIGHT:
            x++;
            if (x > rightBound) {
                x = rightBound;
                rightBound--;
                dir = DOWN;
                y++;
            }
            break;
        case DOWN:
            y++;
            if (y > lowerBound) {
                y = lowerBound;
                lowerBound--;
                dir = LEFT;
                x--;
            }
            break;
        case LEFT:
            x--;
            if (x < leftBound) {
                x = leftBound;
                leftBound++;
                dir = UP;
                y--;
            }
            break;
        case UP:
            y--;
            if (y < upperBound) {
                y = upperBound;
                upperBound++;
                dir = RIGHT;
                x++;
            }
            break;
    }
    if (rightBound - leftBound < 0 && lowerBound - upperBound < 0) {
        x = 0;
        y = 0;
        dir = RIGHT;
        upperBound = 1;
        lowerBound = 7;
        leftBound = 0;
        rightBound = 7;
        mode ^= 1;
    } 
    delay(50);
}
