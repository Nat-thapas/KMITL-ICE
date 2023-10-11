#ifndef SENSORREADER_HPP
#define SENSORREADER_HPP

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
        // for (int r=0; r<round; r++) {
        //     for (int i=0; i<4; i++) {
        //         int sensorVal = analogRead(this->sensorPins[i]);
        //         if (sensorVal < this->sensorMinVals[i]) {
        //             this->sensorMinVals[i] = sensorVal;
        //         }
        //         if (sensorVal > this->sensorMaxVals[i]) {
        //             this->sensorMaxVals[i] = sensorVal;
        //         }
        //     }
        // }
        // for (int i=0; i<4; i++) {
        //     if (this->sensorMinVals[i] >= this->sensorMaxVals[i]) {
        //         return false;
        //     }
        //     this->sensorThresholds[i] = (this->sensorMinVals[i] + this->sensorMaxVals[i]) / 2;
        // }
        // return true;
        for (int i = 0; i < 4; i++) {
            this->sensorMinVals[i] = 300;
            this->sensorMaxVals[i] = 900;
            this->sensorThresholds[i] = 600;
        }
        return true;
    }

    bool getDigitalSensorValue(int sensorIdx) {
        return this->getRawSensorValue(sensorIdx) > this->sensorThresholds[sensorIdx];
    }

    int getDetectionStatus() {
        int sensorsValue = (this->getDigitalSensorValue(0) << 3) | (this->getDigitalSensorValue(1) << 2) | (this->getDigitalSensorValue(2) << 1) | (this->getDigitalSensorValue(3));
        return sensorsValue;
    }

    int getLinePosition() {
        int maxIdx = 0;
        float maxVal = 0;
        for (int i = 0; i < 4; i++) {
            float sensorVal = this->getCalibratedSensorValue(i);
            if (sensorVal >= maxVal) {
                maxVal = sensorVal;
                maxIdx = i;
            }
        }
        return maxIdx;
    }
};

#endif