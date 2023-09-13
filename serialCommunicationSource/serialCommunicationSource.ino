void setup() {
    Serial.begin(9600);
    // Serial1.begin(9600);
}

void loop() {
    while (Serial.available() <= 0) delay(25);
    char b = Serial.read();
    Serial.print(b);
    // if (b == '\n') return;
    // if (b < '0' || b > '9') {
        // Serial.print(b);
        // Serial.println(" is not a number.");
    // } else {
        // Serial.println(b);
        // Serial1.println(b);
    // }
}
