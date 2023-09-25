#define PEDESTRIAN_BUTTON 12
#define NORTH_BUTTON A1
#define EAST_BUTTON A0

const int ledsPin[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

struct state {
    unsigned int outputPattern;
    unsigned long delayTime;
    unsigned int nextStates[8];
};

struct state machines[11] = {
    {0b0100101100, 15000, {1, 1, 0, 1, 1, 1, 1, 1}},
    {0b0101001100, 3000, {2, 2, 4, 2, 5, 5, 5, 5}},
    {0b0110001001, 15000, {3, 2, 3, 3, 3, 3, 3, 3}},
    {0b0110001010, 3000, {0, 4, 0, 0, 5, 5, 5, 5}},
    {0b0110001100, 5000, {0, 2, 0, 0, 5, 5, 5, 5}},
    {0b1010010100, 15000, {6, 6, 6, 6, 5, 5, 5, 5}},
    {0b0010000100, 500, {7, 7, 7, 7, 7, 7, 7, 7}},
    {0b1010010100, 500, {8, 8, 8, 8, 8, 8, 8, 8}},
    {0b0010000100, 500, {9, 9, 9, 9, 9, 9, 9, 9}},
    {0b1010010100, 500, {10, 10, 10, 10, 10, 10, 10, 10}},
    {0b0010000100, 500, {0, 2, 0, 0, 4, 4, 4, 4}}
};

void setup() {
    pinMode(PEDESTRIAN_BUTTON, INPUT_PULLUP);
    pinMode(NORTH_BUTTON, INPUT_PULLUP);
    pinMode(EAST_BUTTON, INPUT_PULLUP);
    for (int i=0; i<10; i++) {
        pinMode(ledsPin[i], OUTPUT);
    }
}

int currentState = 4;

void loop() {
    for (int i = 0; i < 10; i++) {
        digitalWrite(2+i, !bitRead(machines[currentState].outputPattern, 9-i));
    }
    delay(machines[currentState].delayTime);
    bool pedestrianButtonState = !digitalRead(PEDESTRIAN_BUTTON);
    bool northButtonState = !digitalRead(NORTH_BUTTON);
    bool eastButtonState = !digitalRead(EAST_BUTTON);
    int nextStateIndex = (pedestrianButtonState << 2) + (northButtonState << 1) + eastButtonState;
    currentState = machines[currentState].nextStates[nextStateIndex];
}
