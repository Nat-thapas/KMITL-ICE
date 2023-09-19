void setup() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    Serial.begin(9600);
}

void loop() {
    int x = analogRead(A0);
    int y = analogRead(A1);
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);
    delay(100);
}
