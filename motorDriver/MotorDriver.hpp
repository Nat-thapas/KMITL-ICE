#ifndef MOTORDRIVER_HPP
#define MOTORDRIVER_HPP

class MotorDriver {
    int pinA;
    int pinB;
    int maxBackwardPWM;
    int maxForwardPWM;
    bool reverse;

   public:
    MotorDriver(int pinA, int pinB, int maxBackwardPWM, int maxForwardPWM, bool reverse) {
        pinMode(pinA, OUTPUT);
        pinMode(pinB, OUTPUT);
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
        this->pinA = pinA;
        this->pinB = pinB;
        this->maxBackwardPWM = maxBackwardPWM;
        this->maxForwardPWM = maxForwardPWM;
        this->reverse = reverse;
    }

    void setSpeed(float speed) {
        speed = constrain(speed, -1.f, 1.f);
        if ((speed >= 0.f) ^ reverse) {
            analogWrite(pinA, static_cast<int>(abs(speed) * this->maxForwardPWM));
            digitalWrite(pinB, LOW);
        } else {
            digitalWrite(pinA, LOW);
            analogWrite(pinB, static_cast<int>(abs(speed) * this->maxBackwardPWM));
        }
    }
};

#endif  // MOTORDRIVER_HPP