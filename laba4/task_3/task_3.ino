#include <LedControl.h>

const int DIN_PIN = 11;
const int CLK_PIN = 13;
const int CS_PIN  = 10;
const int NUM_DEVICES = 4; 

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, NUM_DEVICES);


const int BUFFER_WIDTH = 32; 
const int BUFFER_HEIGHT = 8;

const long SCROLL_INTERVAL = 100; 
unsigned long previousScrollTime = 0;
int scrollPosition = 0; 
const byte font_A[] = {0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00};
const byte font_R[] = {0x7F, 0x09, 0x19, 0x29, 0x46, 0x00};
const byte font_D[] = {0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00};
const byte font_U[] = {0x7F, 0x40, 0x40, 0x40, 0x7F, 0x00};
const byte font_I[] = {0x00, 0x7F, 0x00, 0x00, 0x00, 0x00};
const byte font_N[] = {0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00};
const byte font_O[] = {0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00};
const byte font_Space[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


const char* message = "ARDUINO ";

void scrollStep() {

  static byte columnBuffer[BUFFER_WIDTH];
  static bool bufferLoaded = false;

  if (!bufferLoaded) {
    int currentCol = 0;
    for (int i = 0; message[i] != '\0'; i++) {
      const byte* charData;
      int charWidth = 6; 

      switch (message[i]) {
        case 'A': charData = font_A; break;
        case 'R': charData = font_R; break;
        case 'D': charData = font_D; break;
        case 'U': charData = font_U; break;
        case 'I': charData = font_I; break;
        case 'N': charData = font_N; break;
        case 'O': charData = font_O; break;
        case ' ': charData = font_Space; break;
        default: charData = font_Space; charWidth = 1; break;
      }
      
      for (int c = 0; c < charWidth; c++) {
        if (currentCol < BUFFER_WIDTH) {
          columnBuffer[currentCol] = charData[c];
          currentCol++;
        }
      }
    }
    while (currentCol < BUFFER_WIDTH) {
      columnBuffer[currentCol] = 0x00;
      currentCol++;
    }
    bufferLoaded = true;
  }
  
  scrollPosition = (scrollPosition + 1) % BUFFER_WIDTH;
  
  for (int device = 0; device < NUM_DEVICES; device++) {
    for (int col = 0; col < 8; col++) {
      int bufferIndex = (scrollPosition + device * 8 + col) % BUFFER_WIDTH;
      lc.setColumn(device, col, columnBuffer[bufferIndex]);
    }
  }
}

void setup() {
  for (int i = 0; i < NUM_DEVICES; i++) {
    lc.shutdown(i, false); 
    lc.setIntensity(i, 4);
    lc.clearDisplay(i);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousScrollTime >= SCROLL_INTERVAL) {
    previousScrollTime = currentMillis;
    
    scrollStep();
  }
}
