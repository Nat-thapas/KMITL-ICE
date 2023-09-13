#define RECEIVE_PIN 2
#define LED_PIN 4

void setup() {
    pinMode(RECEIVE_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, digitalRead(RECEIVE_PIN));
}
