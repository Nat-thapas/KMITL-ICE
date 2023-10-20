void setup() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    // pinMode(A4, INPUT);
    // pinMode(7, INPUT);
    // pinMode(12, OUTPUT);
    // pinMode(13, OUTPUT);
    // digitalWrite(12, LOW);
    // digitalWrite(13, HIGH);
    Serial.begin(115200);
}

void loop() {
    int x = analogRead(A0);
    int y = analogRead(A1);
    int a = analogRead(A2);
    int b = analogRead(A3);
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(a);
    Serial.print(" ");
    Serial.println(b);
    delay(10);
    // Serial.print(analogRead(A4));
    // Serial.print("  ");
    // Serial.println(digitalRead(7));
    // delay(200);
}
