const int buttonPin = 10;
const int ledPin = 13;
int buttonState = 0;
void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    Serial.begin(9600);
}
void loop() {
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Кнопка нажата");
    } else {
    digitalWrite(ledPin, LOW);
    Serial.println("Кнопка отпущена");
    delay(100);
    }
}