#define LED_PIN 4

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    Serial.begin(9600);
    Serial.setTimeout(2147000000);
}

void loop() {
    int n = Serial.parseInt();
    for (int i=0; i<n; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(250);
        digitalWrite(LED_PIN, HIGH);
        delay(250);
    }
}
