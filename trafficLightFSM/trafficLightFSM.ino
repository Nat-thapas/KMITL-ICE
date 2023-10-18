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
    {0b0100101100, 1500, {0, 1, 0, 1, 1, 1, 1, 1}},
    {0b0101001100, 300, {2, 2, 4, 2, 5, 5, 5, 5}},
    {0b0110001001, 1500, {2, 2, 3, 3, 3, 3, 3, 3}},
    {0b0110001010, 300, {0, 4, 0, 0, 5, 5, 5, 5}},
    {0b0110001100, 500, {0, 2, 0, 0, 5, 5, 5, 5}},
    {0b1010010100, 1500, {5, 6, 6, 6, 5, 5, 5, 5}},
    {0b0010000100, 200, {7, 7, 7, 7, 7, 7, 7, 7}},
    {0b1010010100, 200, {8, 8, 8, 8, 8, 8, 8, 8}},
    {0b0010000100, 200, {9, 9, 9, 9, 9, 9, 9, 9}},
    {0b1010010100, 200, {10, 10, 10, 10, 10, 10, 10, 10}},
    {0b0010000100, 200, {0, 2, 0, 0, 4, 4, 4, 4}}
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
    digitalWrite(2+0, !bitRead(machines[currentState].outputPattern, 9-0));
    digitalWrite(2+1, !bitRead(machines[currentState].outputPattern, 9-1));
    digitalWrite(2+2, !bitRead(machines[currentState].outputPattern, 9-2));
    digitalWrite(2+3, !bitRead(machines[currentState].outputPattern, 9-3));
    digitalWrite(2+4, !bitRead(machines[currentState].outputPattern, 9-4));
    digitalWrite(2+5, !bitRead(machines[currentState].outputPattern, 9-5));
    digitalWrite(2+6, !bitRead(machines[currentState].outputPattern, 9-6));
    digitalWrite(2+7, !bitRead(machines[currentState].outputPattern, 9-7));
    digitalWrite(2+8, !bitRead(machines[currentState].outputPattern, 9-8));
    digitalWrite(2+9, !bitRead(machines[currentState].outputPattern, 9-9));
    delay(machines[currentState].delayTime);
    bool pedestrianButtonState = !digitalRead(PEDESTRIAN_BUTTON);
    bool northButtonState = !digitalRead(NORTH_BUTTON);
    bool eastButtonState = !digitalRead(EAST_BUTTON);
    int nextStateIndex = (pedestrianButtonState << 2) + (northButtonState << 1) + eastButtonState;
    currentState = machines[currentState].nextStates[nextStateIndex];
}
