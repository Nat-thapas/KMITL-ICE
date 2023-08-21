#define BUTTON_PIN_1 4
#define BUTTON_PIN_2 3
#define BUTTON_PIN_3 2
#define LED_PIN_1 10
#define LED_PIN_2 9
#define LED_PIN_3 8

void setup() {
    pinMode(BUTTON_PIN_1, INPUT);
    pinMode(BUTTON_PIN_2, INPUT);
    pinMode(BUTTON_PIN_3, INPUT_PULLUP);
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN_1, digitalRead(BUTTON_PIN_1));
    digitalWrite(LED_PIN_2, !digitalRead(BUTTON_PIN_2));
    digitalWrite(LED_PIN_3, !digitalRead(BUTTON_PIN_3));
    delay(50);
}
