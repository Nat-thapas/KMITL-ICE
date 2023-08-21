#define INPUT_PIN A0
#define LED_PIN_START 2

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

void setup() {
    pinMode(INPUT_PIN, INPUT);
    for (int i=0; i<4; i++) {
        pinMode(LED_PIN_START+i, OUTPUT);
    }
    // Serial.begin(9600);
}

int lastPinValue = 1023;

void loop() {
    int pinValue = analogRead(INPUT_PIN);
    while (abs(pinValue - lastPinValue) > 2) {
        pinValue = analogRead(INPUT_PIN);
        lastPinValue = pinValue;
        delay(50);
    }
    // Serial.print(pinValue);
    // Serial.print(" : ");
    int minDiff = 16000;
    int minIdx = -1;
    for (int i=0; i<16; i++) {
        if (abs(pinValue - voltageMap[i][0]) < minDiff) {
            minDiff = abs(pinValue - voltageMap[i][0]);
            minIdx = i;
        }
    }
    int ledStates = voltageMap[minIdx][1];
    // Serial.println(ledStates);
    for (int i=0; i<4; i++) {
        digitalWrite(LED_PIN_START+i, ((ledStates >> (3-i)) & 0b1) ^ 0b1);
    }
    lastPinValue = pinValue;
    delay(50);
}
