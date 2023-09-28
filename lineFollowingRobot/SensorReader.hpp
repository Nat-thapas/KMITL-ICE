#ifndef SENSORREADER_HPP
#define SENSORREADER_HPP

class SensorReader {
    int sensorPins[4];
    int sensorMinVals[4];
    int sensorMaxVals[4];

    int getRawSensorValue(int sensorIdx) {
        return analogRead(this->sensorPins[sensorIdx]);
    }

    float getCalibratedSensorValue(int sensorIdx) {
        return static_cast<float>(analogRead(this->sensorPins[sensorIdx]) - this->sensorMinVals[sensorIdx]) / static_cast<float>(this->sensorMaxVals[sensorIdx] - this->sensorMinVals[sensorIdx]);
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

    void calibrate(int round, int delayTime = 5) {
        for (int i=0; i<round; i++) {
            for (int j=0; j<4; j++) {
                int sensorVal = analogRead(this->sensorPins[i]);
                if (sensorVal < this->sensorMinVals[i]) {
                    this->sensorMinVals[i] = sensorVal;
                }
                if (sensorVal > this->sensorMaxVals[i]) {
                    this->sensorMaxVals[i] = sensorVal;
                }
            }
            delay(delayTime);
        }
    }

    float getLinePosition() {
        float normalSum = 0.f;
        float weightedSum = 0.f;
        for (int i=0; i<4; i++) {
            float sensorVal = this->getCalibratedSensorValue(i);
            Serial.print(sensorVal);
            Serial.print(" ");
            normalSum += sensorVal;
            weightedSum += static_cast<float>(i) * sensorVal;
        }
        Serial.println("");
        return weightedSum / normalSum;
    }
};

#endif