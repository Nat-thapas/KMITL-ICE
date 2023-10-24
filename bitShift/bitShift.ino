#define in1 4
#define in2 5
#define in3 6
#define clck 7

#define led1 8
#define led2 9
#define led3 10
#define led4 11

int readBinary() {
    int output = 0;
    output += (!digitalRead(in1)) * 4;
    output += (!digitalRead(in2)) * 2;
    output += (!digitalRead(in3));
    return output;
}

int circularShift(int num, int count) {
    for (int i = 0; i < count; i++) {
        int firstBit = (num & 0b1000) >> 3;
        num <<= 1;
        num = num & 0b1111;
        num |= firstBit;
    }
    return num;
}

void display(int num) {
    digitalWrite(led1, num & 0b1000);
    digitalWrite(led2, num & 0b0100);
    digitalWrite(led3, num & 0b0010);
    digitalWrite(led4, num & 0b0001);
}

void setup() {
    pinMode(in1, INPUT_PULLUP);
    pinMode(in2, INPUT_PULLUP);
    pinMode(in3, INPUT_PULLUP);
    pinMode(clck, INPUT_PULLUP);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    display(0);
}

int input1;
int input2;

int stage = 0;

void loop() {
    if (!digitalRead(clck)) {
        if (stage == 0) {
            input1 = readBinary();
            display(input1);
            delay(250);
        } else if (stage == 1) {
            input2 = readBinary();
            display(input2);
            delay(250);
        } else {
            input1 = circularShift(input1, input2);
            display(input1);
            delay(250);
        }
        stage++;
    }
}
