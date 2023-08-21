#define BUTTON_PIN_1 2
#define BUTTON_PIN_2 3
#define GREEN_LED_PIN 8
#define YELLOW_LED_PIN 9
#define RED_LED_PIN 10

void setup() {
    pinMode(BUTTON_PIN_1, INPUT_PULLUP);
    pinMode(BUTTON_PIN_2, INPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), redForFive, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), allOffForFive, FALLING);
}

void loop() {
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(7000);
    digitalWrite(GREEN_LED_PIN, LOW);
    for (int i=0; i<6; i++) {
        digitalWrite(YELLOW_LED_PIN, HIGH);
        delay(250);
        digitalWrite(YELLOW_LED_PIN, LOW);
        delay(250);
    }
    digitalWrite(RED_LED_PIN, HIGH);
    delay(5000);
    digitalWrite(RED_LED_PIN, LOW);
}

void redForFive() {
    bool greenLedState = digitalRead(GREEN_LED_PIN);
    bool yellowLedState = digitalRead(YELLOW_LED_PIN);
    bool redLedState = digitalRead(RED_LED_PIN);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    for (int i=0; i<5000; i++) {
        delayMicroseconds(1000);
    }
    digitalWrite(GREEN_LED_PIN, greenLedState);
    digitalWrite(YELLOW_LED_PIN, yellowLedState);
    digitalWrite(RED_LED_PIN, redLedState);
}

void allOffForFive() {
    bool greenLedState = digitalRead(GREEN_LED_PIN);
    bool yellowLedState = digitalRead(YELLOW_LED_PIN);
    bool redLedState = digitalRead(RED_LED_PIN);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    for (int i=0; i<5000; i++) {
        delayMicroseconds(1000);
    }
    digitalWrite(GREEN_LED_PIN, greenLedState);
    digitalWrite(YELLOW_LED_PIN, yellowLedState);
    digitalWrite(RED_LED_PIN, redLedState);
}
