class SsdDriver {
    const int ssdMap[10] = {0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011};
    int ssdPins[8];
    int displaying;
    bool dpState;

   public:
    SsdDriver(int pins[]) {
        for (int i=0; i<8; i++) {
            this->ssdPins[i] = pins[i];
            pinMode(pins[i], OUTPUT);
        }
        this->displaying = -1;
        this->dpState = HIGH;
        update();
    }

    void update() {
        if (displaying < 0 || displaying > 9) {
            for (int i=0; i<7; i++) {
                digitalWrite(ssdPins[i], HIGH);
            }
            digitalWrite(ssdPins[7], !dpState);
            return;
        }
        int ssdVal = ssdMap[displaying];
        for (int i=0; i<7; i++) {
            digitalWrite(ssdPins[i], ((ssdVal >> 6-i) & 0b1) ^ 0b1);
        }
        digitalWrite(ssdPins[7], !dpState);
    }

    void setDisplay(int num) {
        this->displaying = num;
    }

    void setDpState(bool state) {
        this->dpState = state;
    }
};

const int ssdPins[8] = {6, 7, 8, 9, 10, 11, 12, 13};

SsdDriver driver(ssdPins);

void setup() {
    Serial.begin(9600);
}

void loop() {
    while (Serial.available() <= 0) delay(25);
    char b = Serial.read();
    if (b == '\n') return;
    if (b < '0' || b > '9') {
        driver.setDisplay(-1);
        driver.setDpState(true);
        driver.update();
        Serial.print(b);
        Serial.println(" is not a number.");
    } else {
        b &= 0b1111;
        driver.setDisplay(b);
        driver.setDpState(false);
        driver.update();
    }
}