const int buttonPin = 10;
const int ledPin = 13;
const int potPin = A0;
bool ledState = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  Serial.println("Светодиод выключен");
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);
  
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    delay(50);
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    
    if (ledState) {
      Serial.println("Светодиод включен");
    } else {
      Serial.println("Светодиод выключен");
    }
  }
  
  lastButtonState = currentButtonState;
  
  int potValue = analogRead(potPin);
  Serial.print("Потенциометр: ");
  Serial.println(potValue);
  
  delay(100);
}