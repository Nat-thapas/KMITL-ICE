class SsdDriver {
    const int ssdMap[46] = {
        // GFEDCBA  Segments      7-segment map:
        0b0111111, // 0   "0"          AAA
        0b0000110, // 1   "1"         F   B
        0b1011011, // 2   "2"         F   B
        0b1001111, // 3   "3"          GGG
        0b1100110, // 4   "4"         E   C
        0b1101101, // 5   "5"         E   C
        0b1111101, // 6   "6"          DDD
        0b0000111, // 7   "7"
        0b1111111, // 8   "8"
        0b1101111, // 9   "9"
        0b1110111, // 65  'A'
        0b1111100, // 66  'b'
        0b0111001, // 67  'C'
        0b1011110, // 68  'd'
        0b1111001, // 69  'E'
        0b1110001, // 70  'F'
        0b0111101, // 71  'G'
        0b1110110, // 72  'H'
        0b0110000, // 73  'I'
        0b0001110, // 74  'J'
        0b1110110, // 75  'K'  Same as 'H'
        0b0111000, // 76  'L'
        0b0010101, // 77  'M'  Alternate display
        0b1010100, // 78  'n'
        0b0111111, // 79  'O'
        0b1110011, // 80  'P'
        0b1100111, // 81  'q'
        0b1010000, // 82  'r'
        0b1101101, // 83  'S'
        0b1111000, // 84  't'
        0b0111110, // 85  'U'
        0b0111110, // 86  'V'  Same as 'U'
        0b0101010, // 87  'W'  Alternate display
        0b1110110, // 88  'X'  Same as 'H'
        0b1101110, // 89  'y'
        0b1011011, // 90  'Z'  Same as '2'
        0b0000000, // 32  ' '  BLANK
        0b1000000, // 45  '-'  DASH
        0b1100011, // 42 '*'  DEGREE ..
        0b0001000, // 95 '_'  UNDERSCORE
        0b0000001, // LOADING: 1
        0b0000010, // LOADING: 2
        0b0000100, // LOADING: 3
        0b0001000, // LOADING: 4
        0b0010000, // LOADING: 5
        0b0100000, // LOADING: 6
    };
    int ssdPins[8];
    int displaying;
    bool dpState;

   public:
    SsdDriver(int pins[]) {
        for (int i=0; i<8; i++) {
            this->ssdPins[i] = pins[i];
            pinMode(pins[i], OUTPUT);
        }
        this->displaying = -1;
        this->dpState = LOW;
        update();
    }

    void update() {
        if (displaying < 0 || displaying > 45) {
            for (int i=0; i<7; i++) {
                digitalWrite(ssdPins[i], HIGH);
            }
            digitalWrite(ssdPins[7], !dpState);
            return;
        }
        int ssdVal = this->ssdMap[displaying];
        for (int i=0; i<7; i++) {
            digitalWrite(ssdPins[i], ((ssdVal >> i) & 0b1) ^ 0b1);
        }
        digitalWrite(ssdPins[7], !dpState);
    }

    void setDisplay(int num) {
        this->displaying = num;
    }

    void setDpState(bool state) {
        this->dpState = state;
    }
};