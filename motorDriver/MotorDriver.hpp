class MotorDriver {
    int pins[3];
    bool reverse;

   public:
    MotorDriver(int pins[3], bool reverse) {
        for (int i=0; i<3; i++) {
            pinMode(pins[i], OUTPUT);
            this->pins[i] = pins[i];
        }
        analogWrite(this->pins[0], 0);
        this->reverse = reverse;
        if (this->reverse) {
            digitalWrite(pins[1], LOW);
            digitalWrite(pins[2], HIGH);
        } else {
            digitalWrite(pins[1], HIGH);
            digitalWrite(pins[2], LOW);
        }
    }

    void setSpeed(int speed) {
        speed = clamp(speed, -255, 255);
        analogWrite(pins[0], abs(speed));
        if (speed >= 0 ^ reverse) {
            digitalWrite(pins[1], HIGH);
            digitalWrite(pins[2], LOW);
        } else {
            digitalWrite(pins[1], LOW);
            digitalWrite(pins[2], HIGH);
        }
    }
};