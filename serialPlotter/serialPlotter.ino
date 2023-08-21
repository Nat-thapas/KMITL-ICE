#define SAMPLE_PER_PERIOD 24

int mode;

void setup() {
    Serial.begin(9600);
    Serial.print("Please select mode (1: Triangle, 2: Saw tooth, 3: Square): ");
    while (Serial.available() <= 0);
    mode = Serial.read() - '0';
}

int i = 0;

void loop() {
    float x = i/(float)SAMPLE_PER_PERIOD;
    switch(mode) {
        case 1:
            if (x < 0.5f) {
                Serial.println(x*2.f-0.5f);
            } else {
                Serial.println(1.f-(x*2.f-0.5f));
            }
            break;
        case 2:
            Serial.println(x-0.5f);
            break;
        case 3:
        if (x < 0.5f) {
                Serial.println(0.5f);
            } else {
                Serial.println(-0.5f);
            }
            break;
    }
    i = ++i%SAMPLE_PER_PERIOD;
    delay(25);
}
