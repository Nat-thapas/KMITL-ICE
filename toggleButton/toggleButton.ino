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

bool ledState1 = LOW;
bool ledState2 = LOW;
bool ledState3 = LOW;
bool lastButtonState1 = LOW;
bool lastButtonState2 = LOW;
bool lastButtonState3 = LOW;

const unsigned long debounceDelay = 50;  // ms.

unsigned long waitUntil1 = 0;
unsigned long waitUntil2 = 0;
unsigned long waitUntil3 = 0;

void loop() {
    bool buttonState1 = digitalRead(BUTTON_PIN_1);
    bool buttonState2 = !digitalRead(BUTTON_PIN_2);
    bool buttonState3 = !digitalRead(BUTTON_PIN_3);
    if (buttonState1 > lastButtonState1 && millis() > waitUntil1) {
        ledState1 ^= 1;
        waitUntil1 = millis() + debounceDelay;
    }
    if (buttonState2 > lastButtonState2 && millis() > waitUntil2) {
        ledState2 ^= 1;
        waitUntil2 = millis() + debounceDelay;
    }
    if (buttonState3 > lastButtonState3 && millis() > waitUntil3) {
        ledState3 ^= 1;
        waitUntil3 = millis() + debounceDelay;
    }
    lastButtonState1 = buttonState1;
    lastButtonState2 = buttonState2;
    lastButtonState3 = buttonState3;
    digitalWrite(LED_PIN_1, ledState1);
    digitalWrite(LED_PIN_2, ledState2);
    digitalWrite(LED_PIN_3, ledState3);
    delay(50);
}
