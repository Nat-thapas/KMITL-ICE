#define TRANSMIT_PIN 3
#define BUTTON_PIN 4

void setup() {
    pinMode(TRANSMIT_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    digitalWrite(TRANSMIT_PIN, digitalRead(BUTTON_PIN));
}
