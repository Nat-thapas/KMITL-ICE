#include <Servo.h>

#define POTENTIOMETER_PIN A0
#define SERVO_PIN 2
#define LED_PIN 3

Servo servo;

void setup() {
    pinMode(POTENTIOMETER_PIN, INPUT);
    servo.attach(SERVO_PIN);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    Serial.println(potentiometerValue);
    float brightness = (potentiometerValue/64.f)*(potentiometerValue/64.f);
    float angle = potentiometerValue/5.683333;
    servo.write(angle);
    analogWrite(LED_PIN, brightness);
}
