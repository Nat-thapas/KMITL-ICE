class MultiPlexer {
    int commonPins[3];
    int controlPins[3];

    bool displaying[9];

   public:
    MultiPlexer(int *commonPins, int *controlPins) {
        for (int i=0; i<3; i++) {
            this->commonPins[i] = commonPins[i];
            pinMode(commonPins[i], OUTPUT);
            digitalWrite(commonPins[i], LOW);
        }
        for (int i=0; i<3; i++) {
            this->controlPins[i] = controlPins[i];
            pinMode(controlPins[i], OUTPUT);
            digitalWrite(controlPins[i], HIGH);
        }
        for (int i=0; i<9; i++) {
            this->displaying[i] = false;
        }
    }

    void setDisplay(bool *display) {
        for (int i=0; i<9; i++) {
            this->displaying[i] = display[i];
        }
    }

    void display(int delayTime = 5) {
        for (int common = 0; common < 3; common++) {
            for (int control = 0; control < 3; control++) {
                digitalWrite(this->controlPins[control], !this->displaying[common * 3 + control]);
            }
            digitalWrite(this->commonPins[common], HIGH);
            delay(delayTime);
            digitalWrite(this->commonPins[common], LOW);
        }
    }
};


const int commonPins[3] = {8, 9, 10};
const int controlPins[3] = {11, 12, 13};

MultiPlexer matrix(commonPins, controlPins);

bool x[9] = {true, false, true, false, true, false, true, false, true};
bool o[9] = {true, true, true, true, false, true, true, true, true};

void setup() {
}

void loop() {
    if ((millis() / 1000) % 2) {
        matrix.setDisplay(x);
    } else {
        matrix.setDisplay(o);
    }
    matrix.display();
}
