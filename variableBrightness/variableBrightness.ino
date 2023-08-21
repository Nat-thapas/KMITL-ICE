#define POTENTIOMETER_PIN A0
#define LED_PIN 3

void setup() {
    pinMode(POTENTIOMETER_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    Serial.println(potentiometerValue);
    float brightness = (potentiometerValue/64.f)*(potentiometerValue/64.f);
    analogWrite(LED_PIN, brightness);
}
