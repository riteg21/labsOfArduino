const int buttonPin = 10;
const int ledPin = 11;
const int potPin = A0;

int ledBrightness = 0;
int savedBrightness = 0;
bool buttonPressed = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  analogWrite(ledPin, 0);
  Serial.println("Светодиод выключен");
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);
  int potValue = analogRead(potPin);
  
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    delay(50);
    buttonPressed = !buttonPressed;
    
    if (buttonPressed) {
      savedBrightness = ledBrightness;
      Serial.println("Изменение яркости невозможно!");
    } else {
      Serial.println("Изменение яркости разрешено");
    }
  }
  
  if (!buttonPressed) {
    ledBrightness = map(potValue, 0, 1023, 0, 255);
    analogWrite(ledPin, ledBrightness);
    Serial.print("Яркость: ");
    Serial.println(ledBrightness);
  } else {
    analogWrite(ledPin, savedBrightness);
  }
  
  lastButtonState = currentButtonState;
  delay(100);
}