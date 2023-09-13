#include <SoftwareSerial.h>

SoftwareSerial sSerial(10,11); // SoftwareSerial(rxPin, txPin)

void setup() {
    Serial.begin(9600);
    sSerial.begin(4800);
}

bool newlineReceived = true;

void loop() {
    char c;
    if (sSerial.available()) {
        if (newlineReceived) {
            Serial.print("Received > ");
            newlineReceived = false;
        }
        while (sSerial.available()) {
            c = sSerial.read();
            Serial.print(c);
            if (c == '\n') {
                newlineReceived = true;
            }
        }
    }
    if (Serial.available()) {
        Serial.print("Sent > ");
        while (Serial.available()) {
            c = Serial.read();
            Serial.print(c);
            sSerial.print(c);
        }
    }
}