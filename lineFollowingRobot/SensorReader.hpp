#ifndef SENSORREADER_HPP
#define SENSORREADER_HPP

#define NO_LINE_DETECTED     0
#define LINE_DETECTED        1
#define LINES_DETECTED       2
#define SHARP_LEFT_DETECTED  3
#define SHARP_RIGHT_DETECTED 4
#define CROSSING_DETECTED    5

class SensorReader {
    int sensorPins[4];
    int sensorMinVals[4];
    int sensorMaxVals[4];
    int sensorThresholds[4];

    int getRawSensorValue(int sensorIdx) {
        return analogRead(this->sensorPins[sensorIdx]);
    }

    float getCalibratedSensorValue(int sensorIdx) {
        float val = static_cast<float>(this->getRawSensorValue(sensorIdx) - this->sensorMinVals[sensorIdx]) / static_cast<float>(this->sensorMaxVals[sensorIdx] - this->sensorMinVals[sensorIdx]);
        val = constrain(val, 0.f, 1.f);
        return val;
    }

    bool getDigitalSensorValue(int sensorIdx) {
        return this->getRawSensorValue(sensorIdx) > this->sensorThresholds[sensorIdx];
    }

   public: 
    SensorReader(int pins[4]) {
        for (int i=0; i<4; i++) {
            pinMode(pins[i], INPUT);
            this->sensorPins[i] = pins[i];
            this->sensorMinVals[i] = 1023;
            this->sensorMaxVals[i] = 0;
        }
    }

    bool calibrate(int round) {
        for (int r=0; r<round; r++) {
            for (int i=0; i<4; i++) {
                int sensorVal = analogRead(this->sensorPins[i]);
                if (sensorVal < this->sensorMinVals[i]) {
                    this->sensorMinVals[i] = sensorVal;
                }
                if (sensorVal > this->sensorMaxVals[i]) {
                    this->sensorMaxVals[i] = sensorVal;
                }
            }
        }
        for (int i=0; i<4; i++) {
            if (this->sensorMinVals[i] >= this->sensorMaxVals[i]) {
                return false;
            }
            this->sensorThresholds[i] = (this->sensorMinVals[i] + this->sensorMaxVals[i]) / 2;
        }
        return true;
    }

    int getDetectionStatus() {
        int sensorsValue = (this->getDigitalSensorValue(0) << 3) | (this->getDigitalSensorValue(1) << 2) | (this->getDigitalSensorValue(2) << 1) | (this->getDigitalSensorValue(3));
        switch (sensorsValue) {
            case 0b0000:
                return NO_LINE_DETECTED;
                break;
            case 0b1000:
            case 0b0100:
            case 0b0010:
            case 0b0001:
            case 0b0110:
                return LINE_DETECTED;
                break;
            case 0b1001:
            case 0b1101:
            case 0b1011:
            case 0b1010:
            case 0b0101:
                return LINES_DETECTED;
                break;
            case 0b1110:
            case 0b1100:
                return SHARP_LEFT_DETECTED;
                break;
            case 0b0111:
            case 0b0011:
                return SHARP_RIGHT_DETECTED;
                break;
            case 0b1111:
                return CROSSING_DETECTED;
                break;
        }
    }

    float getLinePosition() {
        float normalSum = 0.f;
        float weightedSum = 0.f;
        for (int i=0; i<4; i++) {
            float sensorVal = this->getCalibratedSensorValue(i);
            if (sensorVal >= 0.1f) {
                normalSum += sensorVal;
                weightedSum += static_cast<float>(i) * sensorVal;
            }
        }
        if (normalSum <= 0.f) {
            return 1.5f;
        }
        return weightedSum / normalSum;
    }
};

#endif