#include <Arduino.h>

const uint8_t segPins[] = {12, 10, 6, 8, 9, 11, 5, 7}; 

const uint8_t digitPins[] = {13, 3, 2, 4}; 

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

int numberToDisplay = 1234;

const long refreshInterval = 2000;
int currentDigit = 0;
unsigned long previousRefreshTime = 0;

const long counterInterval = 10;
unsigned long previousCounterTime = 0;
int currentCounterValue = 0;

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
    
    for (int s = 0; s < 8; s++) {
      digitalWrite(segPins[s], (pattern >> s) & 1);
    }

    digitalWrite(digitPins[currentDigit], LOW);
  }
}

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousCounterTime >= counterInterval) {
    previousCounterTime = currentMillis;
    
    currentCounterValue = (currentCounterValue + 1) % 10000;
    numberToDisplay = currentCounterValue;
  }

  dispLayNumber(numberToDisplay);
}
