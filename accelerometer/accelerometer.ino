unsigned long startTime;

void setup() {
    startTime = millis();
    Serial.begin(9600);
}

bool calibrating = true;

int minxaccel = 1023;
int maxxaccel = 0;
int minyaccel = 1023;
int maxyaccel = 0;
int minzaccel = 1023;
int maxzaccel = 0;

void loop() {
    int xaccel = analogRead(A0);
    int yaccel = analogRead(A1);
    int zaccel = analogRead(A2);
    if (calibrating && millis()-startTime >= 10000) {
        calibrating = false;
    }
    if (calibrating) {
        if (xaccel < minxaccel) {
            minxaccel = xaccel;
        }
        if (xaccel > maxxaccel) {
            maxxaccel = xaccel;
        }
        if (yaccel < minyaccel) {
            minyaccel = yaccel;
        }
        if (yaccel > maxyaccel) {
            maxyaccel = yaccel;
        }
        if (zaccel < minzaccel) {
            minzaccel = zaccel;
        }
        if (zaccel > maxzaccel) {
            maxzaccel = zaccel;
        }
        return;
    }
    unsigned long time = millis();
    Serial.print(time);
    Serial.print(" ");
    Serial.print(((float)xaccel-(float)minxaccel)/((float)maxxaccel-(float)minxaccel));
    Serial.print(" ");
    Serial.print(((float)yaccel-(float)minyaccel)/((float)maxyaccel-(float)minyaccel));
    Serial.print(" ");
    Serial.println(((float)zaccel-(float)minzaccel)/((float)maxzaccel-(float)minzaccel));
}