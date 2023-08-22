#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494

#define SPEAKER_PIN 5
#define DEBUG_PIN 13
                       //C  D  E  F  G   A   B
const int notePins[7] = {6, 7, 8, 9, 10, 11, 12};
const int notes[7] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};

void setup() {
    for (int i=0; i<7; i++) {
        pinMode(notePins[i], INPUT_PULLUP);
    }
    pinMode(DEBUG_PIN, INPUT_PULLUP);
    pinMode(SPEAKER_PIN, OUTPUT);
    Serial.begin(9600);
}

unsigned recorded[256] = {};

int currentNote = 0;
int lastNote = 0;

int recordIdx = 0;
bool recording = false;
long recordMillis = 0;

void loop() {
    bool playing = false;
    for (int i=0; i<7; i++) {
        if (!digitalRead(notePins[i])) {
            currentNote = i+1;
            tone(SPEAKER_PIN, notes[i]);
            playing = true;
        }
    }
    if (!playing) {
        currentNote = 0;
        noTone(SPEAKER_PIN);
    }
    if (currentNote != lastNote && recording) {
        int elapsed = millis() - recordMillis;
        if (recordIdx != 0 || recorded[0] & (0b111 < 13) != 0) {
            recorded[recordIdx] |= elapsed & 0b1111111111111;
            recordIdx++;
        }
        recordMillis = millis();
        recorded[recordIdx] = currentNote << 13;
    }
    lastNote = currentNote;
    if (!digitalRead(DEBUG_PIN)) {
        for (int i=0; recorded[i]; i++) {
            Serial.print(recorded[i] >> 13);
            Serial.print(" ");
            Serial.println(recorded[i] & 0b1111111111111);
        }
    }
}



/*
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494

#define SPEAKER_PIN 5
#define DEBUG_PIN 13
#define RECORD_PIN 4
                       //C  D  E  F  G   A   B
const int notePins[7] = {6, 7, 8, 9, 10, 11, 12};
const int notes[7] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};

void setup() {
    for (int i=0; i<7; i++) {
        pinMode(notePins[i], INPUT_PULLUP);
    }
    pinMode(DEBUG_PIN, INPUT_PULLUP);
    pinMode(SPEAKER_PIN, OUTPUT);
    Serial.begin(9600);
}

int recordedNote[256] = {};
long recordedTime[256] = {};

int currentNote = 0;
int lastNote = 0;

int recordIdx = 0;
bool recording = true;
long recordMillis = 0;

void loop() {
    bool playing = false;
    for (int i=0; i<7; i++) {
        if (!digitalRead(notePins[i])) {
            currentNote = i+1;
            tone(SPEAKER_PIN, notes[i]);
            playing = true;
        }
    }
    if (!playing) {
        currentNote = 0;
        noTone(SPEAKER_PIN);
    }
    if (currentNote != lastNote && recording) {
        Serial.println("Different detected!");
        Serial.println(recordIdx);
        int elapsed = millis() - recordMillis;
        if (recordIdx != 0 || recordedNote[0] != 0) {
            recordedTime[recordIdx] = elapsed;
            recordIdx++;
        }
        recordMillis = millis();
        recordedNote[recordIdx] = currentNote;
    }
    lastNote = currentNote;
    if (!digitalRead(DEBUG_PIN)) {
        Serial.println("Begining debug");
        for (int i=0; i<recordIdx; i++) {
            if (recordedNote[i] > 0) {
                Serial.print("Playing: ");
                Serial.println(notes[recordedNote[i]-1]);
                tone(SPEAKER_PIN, notes[recordedNote[i]-1]);
            } else {
                noTone(SPEAKER_PIN);
            }
            Serial.print("Delaying: ");
            Serial.println(recordedTime[i]);
            delay(recordedTime[i]);
        }
        noTone(SPEAKER_PIN);
      delay(1000);
    }
}
*/
