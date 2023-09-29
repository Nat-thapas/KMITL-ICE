#include "MotorDriver.hpp"
#include "SensorReader.hpp"

// Base robot speed, the robot will (probably) not go faster than this
#define baseMotorSpeed 0.4f

// Parameters for the PID controller
#define Kp 10.f
#define Ki 0.f
#define Kd 2.5f
#define PID_DIVIDER 10.f

MotorDriver leftMotor(9, 10, 255, 255, true);
MotorDriver rightMotor(3, 11, 255, 255, true);

const int sensorPins[4] = {A0, A1, A2, A3};
SensorReader sensors(sensorPins);

float lastError = 0.f;
float totalError = 0.f;
float lastExecTime;

void setup() {
    // https://nerdytechy.com/how-to-change-the-pwm-frequency-of-arduino/
    // Pins D9 and D10 - 7.8 kHz
    TCCR1A = 0b00000001; // 8bit
    TCCR1B = 0b00001010; // x8 fast pwm
    // Pins D3 and D11 - 7.8 kHz
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
    Serial.println("Starting calibration sequence...");
    while (!sensors.calibrate(5000)) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Calibration sequence failed, retrying...");
    }
    Serial.println("Calibration sequence succeeded");
    digitalWrite(LED_BUILTIN, LOW);
    lastExecTime = millis();
}

void loop() {
    float deltaTime = millis() - lastExecTime;
    lastExecTime = millis();

    int sensorStatus = sensors.getDetectionStatus();
    if (sensorStatus == NO_LINE_DETECTED) return;
    if (sensorStatus == LINES_DETECTED) digitalWrite(LED_BUILTIN, HIGH);
    else digitalWrite(LED_BUILTIN, LOW);
    if (sensorStatus == CROSSING_DETECTED) {
        leftMotor.setSpeed(-baseMotorSpeed);
        rightMotor.setSpeed(-baseMotorSpeed);
        delay(250);
        rightMotor.setSpeed(0.f);
        leftMotor.setSpeed(0.f);
    }

    // PID calculations
    float linePosition = sensors.getLinePosition();
    Serial.println(linePosition);
    float error = linePosition - 1.5f;
    totalError += error * deltaTime;;
    float dError = (error - lastError) / deltaTime;
    float Pval = Kp * error;
    float Ival = Ki * totalError;
    float Dval = Kd * dError;
    float PIDval = (Pval + Ival + Dval) / PID_DIVIDER;
    lastError = error;
    
    float leftMotorSpeed = baseMotorSpeed;
    float rightMotorSpeed = baseMotorSpeed;

    if (PIDval < 0) {
        leftMotorSpeed -= PIDval;
        leftMotorSpeed = constrain(leftMotorSpeed, -baseMotorSpeed, baseMotorSpeed);
    } else {
        rightMotorSpeed -= PIDval;
        rightMotorSpeed = constrain(rightMotorSpeed, -baseMotorSpeed, baseMotorSpeed);
    }

    leftMotor.setSpeed(leftMotorSpeed);
    rightMotor.setSpeed(rightMotorSpeed);
}