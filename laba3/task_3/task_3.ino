uint32_t lastDebounceTime[4][4] = {0};
const uint16_t DEBOUNCE_DELAY = 10; // 10ms
bool lastKeyState[4][4] = {{false}};
bool currentKeyState[4][4] = {{false}};

const int rowPins[4] = {8, 7, 6, 5};
const int colPins[4] = {11, 10, 9, 4};
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char keyValue;
volatile byte currentRow = 0;
volatile bool keyDetected = false;
volatile uint32_t isrTime = 0;  // Счетчик времени для прерываний

void setupKeyboardPins() {
  for(int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  for(int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

ISR(TIMER1_COMPA_vect) {
  isrTime++;  // Увеличиваем счетчик времени
  
  digitalWrite(rowPins[currentRow], HIGH);
  currentRow = (currentRow + 1) % 4;
  digitalWrite(rowPins[currentRow], LOW);
  
  for (int col = 0; col < 4; col++) {
    bool rawState = (digitalRead(colPins[col]) == LOW);
    
    // Антидребезг
    if (rawState != lastKeyState[currentRow][col]) {
      lastDebounceTime[currentRow][col] = isrTime;
    }
    
    if ((isrTime - lastDebounceTime[currentRow][col]) > DEBOUNCE_DELAY) {
      if (currentKeyState[currentRow][col] != rawState) {
        currentKeyState[currentRow][col] = rawState;
        
        if (rawState) {
          keyDetected = true;
          keyValue = keys[currentRow][col];
        }
      }
    }
    
    lastKeyState[currentRow][col] = rawState;
  }
}

void setup() {
  Serial.begin(9600);
  setupKeyboardPins();
  
  // Настройка Timer1 для прерываний 1kHz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  OCR1A = 249;  // 1ms при 16MHz/64
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  
  sei();
}

void loop() {
  if (keyDetected) {
    Serial.print("Клавиша: ");
    Serial.println(keyValue);
    keyDetected = false;
  }
}