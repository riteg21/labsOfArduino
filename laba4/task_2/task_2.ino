#include <LedControl.h>

const int DIN_PIN = 11;
const int CLK_PIN = 13;
const int CS_PIN  = 10;
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

unsigned long previousMillis = 0;
const long animationInterval = 100;
const long staticDisplayDuration = 3000;

int currentOffset = 0;
bool isAnimating = false;
unsigned long stateStartTime = 0;

const byte heartPattern[8] = {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00010000
};


void displayStaticPattern() {
  lc.clearDisplay(0);
  for (int i = 0; i < 8; i++) {
    lc.setLed(0, i, i, true);
  }
}

void displayAnimationFrame(int offset) {
  lc.clearDisplay(0);
  for (int row = 0; row < 8; row++) {
    byte rowData = heartPattern[row];
    for (int col = 0; col < 8; col++) {
      bool state = (rowData >> (7 - ((col + offset) % 8))) & 0x01;
      lc.setLed(0, row, col, state);
    }
  }
}


void setup() {
  lc.shutdown(0, false); 
  lc.setIntensity(0, 8); 
  lc.clearDisplay(0);
  
  displayStaticPattern();
  isAnimating = false;
  stateStartTime = millis();
}


void loop() {
  unsigned long currentMillis = millis();

  if (isAnimating) {
    if (currentMillis - previousMillis >= animationInterval) {
      previousMillis = currentMillis;

      displayAnimationFrame(currentOffset);

      currentOffset = (currentOffset + 1) % 8;
    }
    if (currentMillis - stateStartTime >= 1000) { 
      displayStaticPattern();
      isAnimating = false;
      stateStartTime = currentMillis;
    }
    
  } else {
    if (currentMillis - stateStartTime >= staticDisplayDuration) {
      isAnimating = true;
      stateStartTime = currentMillis;
      currentOffset = 0;
    }
  }
}
