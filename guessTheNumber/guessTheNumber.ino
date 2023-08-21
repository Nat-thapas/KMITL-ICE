#define INPUT_PIN A0

class SsdDriver {
    const int ssdMap[12] = {0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011, 0b0001110, 0b1000110};
    int ssdPins[8];
    int displaying;
    bool dpState;

    void update() {
        if (displaying < 0 || displaying > 11) {
            for (int i=0; i<6; i++) {
                digitalWrite(ssdPins[i], HIGH);
            }
            digitalWrite(ssdPins[6], LOW);
            digitalWrite(ssdPins[7], dpState);
            return;
        }
        int ssdVal = ssdMap[displaying];
        for (int i=0; i<7; i++) {
            digitalWrite(ssdPins[i], ((ssdVal >> 6-i) & 0b1) ^ 0b1);
        }
        digitalWrite(ssdPins[7], dpState);
    }

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

    void setDisplay(int num) {
        this->displaying = num;
        update();
    }

    void toggleDpState() {
        this->dpState ^= 0b1;
        update();
    }
};

int readButtons(int pin) {
    const int voltageMap[16][2] = {
        {1023, 0b0000},
        {815, 0b0001},
        {682, 0b0010},
        {582, 0b0011},
        {511, 0b0100},
        {453, 0b0101},
        {408, 0b0110},
        {370, 0b0111},
        {335, 0b1000},
        {309, 0b1001},
        {287, 0b1010},
        {268, 0b1011},
        {252, 0b1100},
        {237, 0b1101},
        {224, 0b1110},
        {212, 0b1111}
    };
    int pinValue = analogRead(pin);
    int minDiff = 16000;
    int minIdx = -1;
    for (int i=0; i<16; i++) {
        if (abs(pinValue - voltageMap[i][0]) < minDiff) {
            minDiff = abs(pinValue - voltageMap[i][0]);
            minIdx = i;
        }
    }
    int states = voltageMap[minIdx][1];
    return states;
}

const int ssdPins[8] = {6, 7, 8, 9, 10, 11, 12, 13};

SsdDriver driver(ssdPins);

void setup() {
    pinMode(INPUT_PIN, INPUT);
    randomSeed(analogRead(A5));
    Serial.begin(9600);
}

void loop() {
    int target = random(1, 10);
    Serial.print("Current target: ");
    Serial.println(target);
    int currGuess = 1;
    int buttonsState;
    bool buttonState1 = 0;
    bool buttonState2 = 0;
    bool lastButtonState1 = 0;
    bool lastButtonState2 = 0;
    driver.setDisplay(currGuess);
    while (1) {
        while (1) {
            buttonsState = readButtons(INPUT_PIN);
            buttonState1 = buttonsState & 0b1;
            buttonState2 = buttonsState & 0b10;
            if (buttonState1 > lastButtonState1) {
                currGuess++;
                currGuess = (currGuess-1)%9+1;
                driver.setDisplay(currGuess);
            }
            if (buttonState2 > lastButtonState2) {
                break;
            }
            lastButtonState1 = buttonState1;
            lastButtonState2 = buttonState2;
            delay(50);
        }
        if (currGuess > target) {
            driver.setDisplay(11);
            delay(2500);
        } else if (currGuess < target) {
            driver.setDisplay(10);
            delay(2500);
        } else {
            driver.setDisplay(0);
            delay(2500);
            break;
        }
        driver.setDisplay(currGuess);
    }
}
