#define LED_ARRAY_START 8
#define LED_ARRAY_END 11


void setup() {
    for (int i=LED_ARRAY_START; i<=LED_ARRAY_END; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop() {
    for (int stage=0; stage<=LED_ARRAY_END-LED_ARRAY_START+1; stage++) {
        for (int i=0; i<=LED_ARRAY_END-LED_ARRAY_START; i++) {
            if (i < stage) {
                digitalWrite(LED_ARRAY_START+i, HIGH);
            } else {
                digitalWrite(LED_ARRAY_START+i, LOW);
            }
        }
        delay(500);
    }
}
