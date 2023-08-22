#include "pitches.h"

#define SPEAKER_PIN 4
                       //C  D  E  F  G  A   B
const int notePins[7] = {5, 6, 7, 8, 9, 10, 12};
const int notes[7] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};

void setup() {
    for (int i=0; i<7; i++) {
        pinMode(notePins[i], INPUT_PULLUP);
    }
    pinMode(SPEAKER_PIN, OUTPUT);
}

char currentNote = 'X';

void loop() {
    bool playing = false;
    for (int i=0; i<7; i++) {
        if (!digitalRead(notePins[i])) {
            tone(SPEAKER_PIN, notes[i]);
            playing = true;
        }
    }
    if (!playing) {
        noTone(SPEAKER_PIN);
    }
}
