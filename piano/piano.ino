#include "pitches.h"
#include "ssd.h"

#define SPEAKER_PIN 5
#define MODE_PIN 13
#define ACTION_PIN 4

const int ssdPins[8] = {2, 3, A0, A1, A2, A3, A4, A5};
                       //C  D  E  F  G   A   B
const int notePins[7] = {6, 7, 8, 9, 10, 11, 12};
const int notes[49] = {
    NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,
    NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,
    NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,
    NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,
    NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7
};

SsdDriver ssd(ssdPins);

void setup() {
    for (int i=0; i<7; i++) {
        pinMode(notePins[i], INPUT_PULLUP);
    }
    pinMode(MODE_PIN, INPUT_PULLUP);
    pinMode(ACTION_PIN, INPUT_PULLUP);
    pinMode(SPEAKER_PIN, OUTPUT);
    // Serial.begin(9600);
}

int recordedNotes[128] = {};
long recordedLength[128] = {};

int noteSpace = 3;

int currentNote = 0;
int lastNote = 0;

bool recording = false;
int recordIdx = 0;
long recordMillis = 0;

int modeIdx = 0;
char modes[4] = "nrpc";
char mode = 'n';

void loop() {
    // Drive ssd
    switch (mode) {
        case 'n':
            ssd.setDisplay(23);
            ssd.update();
            break;
        case 'r':
            if (recording) {
                ssd.setDisplay(40 + (millis()/500)%6);
            } else {
                ssd.setDisplay(27);
            }
            ssd.update();
            break;
        case 'p':
            ssd.setDisplay(25);
            ssd.update();
            break;
        case 'c':
            ssd.setDisplay(12);
            ssd.update();
            break;
        default:
            mode = 'n';
            ssd.setDisplay(23);
            ssd.update();
            break;
    }

    // Select mode
    if (!digitalRead(MODE_PIN)) {
        if (recording) {
            recording = false;
        }
        modeIdx++;
        modeIdx %= 4;
        mode = modes[modeIdx];
        delay(1000);
    }

    // Normal playing
    bool playing = false;
    for (int i=0; i<7; i++) {
        if (!digitalRead(notePins[i])) {
            currentNote = 7*noteSpace + i+1;
            tone(SPEAKER_PIN, notes[7*noteSpace + i]);
            playing = true;
        }
    }
    if (!playing) {
        currentNote = 0;
        noTone(SPEAKER_PIN);
    }

    // Config
    if (mode == 'c' && !digitalRead(ACTION_PIN)) {
        noteSpace++;
        noteSpace %= 7;
        ssd.setDisplay(noteSpace);
        ssd.update();
        delay(1000);
    }

    // Recording
    if (mode == 'r' && !digitalRead(ACTION_PIN)) {
        if (recording) {
            recording = false;
        } else {
            recording = true;
            recordIdx = 0;
            for (int i=0; i<128; i++) {
                recordedNotes[i] = 0;
                recordedLength[i] = 0;
            }
        }
        delay(1000);
    }
    if (currentNote != lastNote && recording) {
        // Serial.println("Different detected!");
        // Serial.println(recordIdx);
        int elapsed = millis() - recordMillis;
        if (recordIdx != 0 || recordedNotes[0] != 0) {
            recordedLength[recordIdx] = elapsed;
            recordIdx++;
        }
        recordMillis = millis();
        recordedNotes[recordIdx] = currentNote;
    }
    lastNote = currentNote;

    // Playback
    if (mode == 'p' && !digitalRead(ACTION_PIN)) {
        // Serial.println("Begining playback");
        for (int i=0; i<recordIdx; i++) {
            if (recordedNotes[i] > 0) {
                // Serial.print("Playing: ");
                // Serial.println(notes[recordedNote[i]-1]);
                tone(SPEAKER_PIN, notes[recordedNotes[i]-1]);
            } else {
                noTone(SPEAKER_PIN);
            }
            // Serial.print("Delaying: ");
            // Serial.println(recordedTime[i]);
            delay(recordedLength[i]);
        }
        noTone(SPEAKER_PIN);
    }
    delay(50);
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

int recordedNote[128] = {};
long recordedTime[128] = {};

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
