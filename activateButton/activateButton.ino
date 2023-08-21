#define BUTTON_PIN_1 4
#define BUTTON_PIN_2 3
#define BUTTON_PIN_3 2
#define LED_PIN_1 10
#define LED_PIN_2 9
#define LED_PIN_3 8

bool ledState1 = LOW;
bool ledState2 = LOW;
bool ledState3 = LOW;

void setup() {
    pinMode(BUTTON_PIN_1, INPUT);
    pinMode(BUTTON_PIN_2, INPUT);
    pinMode(BUTTON_PIN_3, INPUT_PULLUP);
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
}

void loop() {
    ledState1 |= digitalRead(BUTTON_PIN_1);
    ledState2 |= !digitalRead(BUTTON_PIN_2);
    ledState3 |= !digitalRead(BUTTON_PIN_3);
    digitalWrite(LED_PIN_1, ledState1);
    digitalWrite(LED_PIN_2, ledState2);
    digitalWrite(LED_PIN_3, ledState3);
    delay(50);
}
