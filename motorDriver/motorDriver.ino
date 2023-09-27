#include "MotorDriver.hpp"

MotorDriver leftMotor(9, 10, 255, 255, true);

MotorDriver rightMotor(3, 11, 255, 255, true);

void setup() {
    // https://nerdytechy.com/how-to-change-the-pwm-frequency-of-arduino/
    // Pins D9 and D10 - 7.8 kHz
    TCCR1A = 0b00000001; // 8bit
    TCCR1B = 0b00001010; // x8 fast pwm
    // Pins D3 and D11 - 8 kHz
    TCCR2B = 0b00000010; // x8
    TCCR2A = 0b00000011; // fast pwm
    // // Pins D9 and D10 - 31.4 kHz
    // TCCR1A = 0b00000001; // 8bit
    // TCCR1B = 0b00000001; // x1 phase correct
    // // Pins D3 and D11 - 31.4 kHz
    // TCCR2B = 0b00000001; // x1
    // TCCR2A = 0b00000001; // phase correct
}

void loop() {
    leftMotor.setSpeed(0.5f);
    rightMotor.setSpeed(0.5f);
    delay(1500);
    rightMotor.setSpeed(-0.5f);
    delay(364);
}