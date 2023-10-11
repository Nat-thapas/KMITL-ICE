#include "MotorDriver.hpp"
#include "SensorReader.hpp"

#define LDR_PIN A4

// Base robot speed, the robot will (probably) not go faster than this
#define baseMotorSpeed 0.6f

// Parameters for the PID controller  15 0 0.35: a little oscilation
#define Kp 25.f
#define Ki 0.f
#define Kd 0.5f
#define PID_DIVIDER 10.f

MotorDriver leftMotor(9, 10, 255, 255, true);
MotorDriver rightMotor(3, 11, 255, 255, true);

const int sensorPins[4] = {A0, A1, A2, A3};
SensorReader sensors(sensorPins);

float lastError = 0.f;
float totalError = 0.f;
unsigned long lastExecTime;

int ldrBaseVal = 0;

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
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
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

bool turnedAtCrossing = false;
bool turnedRightAtCrossing = false;

void loop() {
    float deltaTime = static_cast<float>(millis() - lastExecTime) / 1000.f;
    lastExecTime = millis();

    float error = sensors.getLinePosition() - 1.5f;
    int sensorStatus = sensors.getDetectionStatus();

    switch (sensorStatus) {
        case 0b0000:
            error = 0.f;
            break;
        case 0b0001:
        case 0b0011:
            leftMotor.setSpeed(baseMotorSpeed * 0.8f);
            rightMotor.setSpeed(baseMotorSpeed * 0.8f);
            delay(500);
            leftMotor.setSpeed(baseMotorSpeed * 0.75f);
            rightMotor.setSpeed(-baseMotorSpeed * 0.75f);
            delay(100);
            while (!sensors.getDigitalSensorValue(1)) delay(2);
            return;
            break;
        case 0b1000:
        case 0b1100:
            leftMotor.setSpeed(baseMotorSpeed * 0.8f);
            rightMotor.setSpeed(baseMotorSpeed * 0.8f);
            delay(500);
            leftMotor.setSpeed(-baseMotorSpeed * 0.75f);
            rightMotor.setSpeed(baseMotorSpeed * 0.75f);
            delay(100);
            while (!sensors.getDigitalSensorValue(2)) delay(2);
            return;
            break;
        case 0b1111:
            if (turnedAtCrossing) {
                if (turnedRightAtCrossing) {
                    leftMotor.setSpeed(baseMotorSpeed * 0.8f);
                    rightMotor.setSpeed(baseMotorSpeed * 0.8f);
                    delay(200);
                    leftMotor.setSpeed(baseMotorSpeed * 0.75f);
                    rightMotor.setSpeed(-baseMotorSpeed * 0.75f);
                    delay(500);
                    while (!sensors.getDigitalSensorValue(1)) delay(2);
                    return;
                } else {
                    leftMotor.setSpeed(baseMotorSpeed * 0.8f);
                    rightMotor.setSpeed(baseMotorSpeed * 0.8f);
                    delay(200);
                    leftMotor.setSpeed(-baseMotorSpeed * 0.75f);
                    rightMotor.setSpeed(baseMotorSpeed * 0.75f);
                    delay(500);
                    while (!sensors.getDigitalSensorValue(2)) delay(2);
                    return;
                }
            }
            leftMotor.setSpeed(0.f);
            rightMotor.setSpeed(0.f);
            turnedAtCrossing = true;
            delay(500);
            int blinks = 0;
            unsigned long startTime = millis();
            bool lastLdrVal = analogRead(LDR_PIN) < 250;
            while (millis() - startTime < 5000) {
                bool ldrVal = analogRead(LDR_PIN) < 250;
                // Serial.println(analogRead(LDR_PIN));
                // digitalWrite(LED_BUILTIN, ldrVal);
                if (ldrVal > lastLdrVal) {
                    blinks++;
                }
                lastLdrVal = ldrVal;
            }
            if (blinks <= 1) {
                leftMotor.setSpeed(baseMotorSpeed * 0.8f);
                rightMotor.setSpeed(baseMotorSpeed * 0.8f);
                delay(400);
                leftMotor.setSpeed(-baseMotorSpeed * 0.75f);
                rightMotor.setSpeed(baseMotorSpeed * 0.75f);
                delay(100);
                while (!sensors.getDigitalSensorValue(2)) delay(2);
                turnedRightAtCrossing = false;
                return;
            } else {
                leftMotor.setSpeed(baseMotorSpeed * 0.8f);
                rightMotor.setSpeed(baseMotorSpeed * 0.8f);
                delay(400);
                leftMotor.setSpeed(baseMotorSpeed * 0.75f);
                rightMotor.setSpeed(-baseMotorSpeed * 0.75f);
                delay(100);
                while (!sensors.getDigitalSensorValue(1)) delay(2);
                turnedRightAtCrossing = true;
                return;
            }
            break;
        default:
            break;
    }

    // PID calculations
    // Serial.print("Error: ");
    // Serial.print(error);
    totalError += error * deltaTime;;
    float dError = (error - lastError) / deltaTime;
    float Pval = Kp * error;
    float Ival = Ki * totalError;
    float Dval = Kd * dError;
    float PIDval = (Pval + Ival + Dval) / PID_DIVIDER;
    lastError = error;
    // Serial.print("   P: ");
    // Serial.print(Pval);
    // Serial.print(" I: ");
    // Serial.print(Ival);
    // Serial.print(" D: ");
    // Serial.print(Dval);
    // Serial.print("   PID: ");
    // Serial.println(PIDval);
    
    float leftMotorSpeed = baseMotorSpeed;
    float rightMotorSpeed = baseMotorSpeed;

    if (PIDval < 0) {
        leftMotorSpeed += PIDval;
        leftMotorSpeed = constrain(leftMotorSpeed, -baseMotorSpeed, baseMotorSpeed);
    } else {
        rightMotorSpeed -= PIDval;
        rightMotorSpeed = constrain(rightMotorSpeed, -baseMotorSpeed, baseMotorSpeed);
    }

    leftMotor.setSpeed(leftMotorSpeed);
    rightMotor.setSpeed(rightMotorSpeed);

    ldrBaseVal = (ldrBaseVal + analogRead(LDR_PIN)) / 2; 

    delay(2);
}