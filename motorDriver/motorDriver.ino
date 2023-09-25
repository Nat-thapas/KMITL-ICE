#include "clamp.hpp"
#include "MotorDriver.hpp"

int leftMotorPins[3] = {3, 2, 4};
MotorDriver leftMotor(leftMotorPins, false);

int rightMotorPins[3] = {6, 5, 7};
MotorDriver rightMotor(rightMotorPins, false);

void setup() {
}

void loop() {
    leftMotor.setSpeed(255);
    rightMotor.setSpeed(255);
}