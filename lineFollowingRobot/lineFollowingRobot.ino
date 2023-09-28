#include "MotorDriver.hpp"
#include "SensorReader.hpp"

#define baseMotorSpeed 0.4f;

MotorDriver leftMotor(9, 10, 255, 255, true);
MotorDriver rightMotor(3, 11, 255, 255, true);

const int sensorPins[4] = {A0, A1, A2, A3};
SensorReader sensors(sensorPins);

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
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Starting calibration sequence...")
    while (!sensors.calibrate(5000)) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Calibration sequence failed, retrying...");
    }
    Serial.println("Calibration sequence succeeded");
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    float linePosition = sensors.getLinePosition();
    Serial.println(linePosition);
}