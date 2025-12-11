const int buttonPin = 2; 
const int ledPin = 13;
volatile bool stateChanged = false;
uint32_t timer_blink;
bool interrupts_flag = false;
unsigned long stateStart = 0;
bool ledState = false;

// Добавляем переменные для антидребезга
unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 50; // 50ms для антидребезга

void handleButtonPress() {
  unsigned long currentTime = millis();
  // Игнорируем срабатывания, которые происходят слишком быстро
  if (currentTime - lastInterruptTime > debounceDelay) {
    stateChanged = true;
  }
  lastInterruptTime = currentTime;
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
}

void loop() {
  if (stateChanged) {
    interrupts_flag = !interrupts_flag;
    stateChanged = false;
    stateStart = millis();
  }
  
  timer_blink = interrupts_flag ? 200 : 1000;
  
  if (millis() - stateStart >= timer_blink) {
    stateStart = millis();
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}