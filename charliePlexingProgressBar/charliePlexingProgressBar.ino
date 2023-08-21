#define LED_PIN_A 8
#define LED_PIN_B 9
#define LED_PIN_C 10

class CharliePlexer {
    int pinA, pinB, pinC;
    bool ledStates[6];

   public:
    CharliePlexer(int a, int b, int c) {
        pinA = a;
        pinB = b;
        pinC = c;
        pinMode(pinA, INPUT);
        pinMode(pinB, INPUT);
        pinMode(pinC, INPUT);
        for (int i=0; i<6; i++) {
            ledStates[i] = false;
        }
    }

    setLedState(int ledIndex, bool state) {
        ledStates[ledIndex] = state;
    }

    display(int delayTime, int nopDelayTime = -1) {
        if (nopDelayTime < 0) {
            nopDelayTime = delayTime;
        }
        for (int i=0; i<6; i++) {
            pinMode(pinA, INPUT);
            pinMode(pinB, INPUT);
            pinMode(pinC, INPUT);
            if (!ledStates[i]) {
                if (nopDelayTime > 0) {
                    delay(nopDelayTime);
                }
                continue;
            }
            switch (i) {
                case 0:
                    pinMode(pinA, OUTPUT);
                    pinMode(pinB, OUTPUT);
                    digitalWrite(pinA, HIGH);
                    digitalWrite(pinB, LOW);
                    break;
                case 1:
                    pinMode(pinA, OUTPUT);
                    pinMode(pinB, OUTPUT);
                    digitalWrite(pinB, HIGH);
                    digitalWrite(pinA, LOW);
                    break;
                case 2:
                    pinMode(pinA, OUTPUT);
                    pinMode(pinC, OUTPUT);
                    digitalWrite(pinA, HIGH);
                    digitalWrite(pinC, LOW);
                    break;
                case 3:
                    pinMode(pinA, OUTPUT);
                    pinMode(pinC, OUTPUT);
                    digitalWrite(pinC, HIGH);
                    digitalWrite(pinA, LOW);
                    break;
                case 4:
                    pinMode(pinB, OUTPUT);
                    pinMode(pinC, OUTPUT);
                    digitalWrite(pinB, HIGH);
                    digitalWrite(pinC, LOW);
                    break;
                case 5:
                    pinMode(pinB, OUTPUT);
                    pinMode(pinC, OUTPUT);
                    digitalWrite(pinC, HIGH);
                    digitalWrite(pinB, LOW);
                    break;
            }
            delay(delayTime);
        }
        pinMode(pinA, INPUT);
        pinMode(pinB, INPUT);
        pinMode(pinC, INPUT);
    }
};

CharliePlexer charliePlexer(LED_PIN_A, LED_PIN_B, LED_PIN_C);

void setup() {}

void loop() {
    int ledOnCount = (millis()/500)%7;
    for (int i=0; i<6; i++) {
        charliePlexer.setLedState(i, false);
    }
    for (int i=0; i<ledOnCount; i++) {
        charliePlexer.setLedState(i, true);
    }
    charliePlexer.display(1);
}
