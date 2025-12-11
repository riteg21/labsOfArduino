#include <Arduino.h>

const int LATCH_PIN = 10;
const int CLOCK_PIN = 11;
const int DATA_PIN  = 12;

const uint8_t digitPins[] = {2, 3, 4, 5}; 

const uint8_t digits[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01100111
};

const long refreshInterval = 2000;
int currentDigit = 0;
unsigned long previousRefreshTime = 0;

void writeSegments(uint8_t pattern) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  digitalWrite(LATCH_PIN, HIGH);
}

void dispLayNumber(int vaLue) {
  int d[4];
  d[0] = vaLue % 10;
  d[1] = (vaLue / 10) % 10;
  d[2] = (vaLue / 100) % 10;
  d[3] = (vaLue / 1000) % 10;

  unsigned long currentMicros = micros();

  if (currentMicros - previousRefreshTime >= refreshInterval / 4) {
    previousRefreshTime = currentMicros;

    digitalWrite(digitPins[currentDigit], HIGH);

    currentDigit = (currentDigit + 1) % 4;

    uint8_t pattern = digits[d[currentDigit]];
    writeSegments(pattern);

    digitalWrite(digitPins[currentDigit], LOW);
  }
}

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }

  writeSegments(0);
}

void loop() {
  int displayValue = (int)(millis() % 10000);
  
  dispLayNumber(displayValue);
}