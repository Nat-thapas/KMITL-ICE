void setup() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    Serial.begin(115200);
}

void loop() {
    int x = analogRead(A0);
    int y = analogRead(A1);
    int a = analogRead(A2);
    int b = analogRead(A3);
    int c = analogRead(A4);
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.println(c);
    // Serial.println(analogRead(A4));
    delay(10);
}
