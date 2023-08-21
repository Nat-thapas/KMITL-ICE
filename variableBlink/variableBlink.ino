#define POTENTIOMETER_PIN A0

void setup() {
    pinMode(POTENTIOMETER_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    Serial.println(potentiometerValue);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(potentiometerValue+50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(potentiometerValue);
}
