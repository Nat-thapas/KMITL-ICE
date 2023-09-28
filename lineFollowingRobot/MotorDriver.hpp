#ifndef MOTORDRIVER_HPP
#define MOTORDRIVER_HPP

class MotorDriver {
    int pinA;
    int pinB;
    int maxBackwardPWM;
    int maxForwardPWM;

   public:
    MotorDriver(int pinA, int pinB, int maxBackwardPWM, int maxForwardPWM, bool reverse) {
        pinMode(pinA, OUTPUT);
        pinMode(pinB, OUTPUT);
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
        if (reverse) {
            this->pinA = pinB;
            this->pinB = pinA;
        } else {
            this->pinA = pinA;
            this->pinB = pinB;
        }
        this->maxBackwardPWM = maxBackwardPWM;
        this->maxForwardPWM = maxForwardPWM;
    }

    void setSpeed(float speed) {
        speed = constrain(speed, -1.f, 1.f);
        if (speed >= 0.f) {
            analogWrite(pinA, static_cast<int>(speed * this->maxForwardPWM));
            digitalWrite(pinB, LOW);
        } else {
            digitalWrite(pinA, LOW);
            analogWrite(pinB, static_cast<int>(-speed * this->maxBackwardPWM));
        }
    }
};

#endif  // MOTORDRIVER_HPP