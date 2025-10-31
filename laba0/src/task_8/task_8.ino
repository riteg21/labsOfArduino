const int buttonPin = 10;
const int greenLed = 11;
const int yellowLed = 12;
const int redLed = 13;

bool buttonPressed = false;
bool lastButtonState = HIGH;
bool trafficLightActive = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);
  
  digitalWrite(greenLed, HIGH);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  Serial.println("Ожидание нажатия кнопки...");
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);
  
  if (currentButtonState == LOW && lastButtonState == HIGH && !trafficLightActive) {
    delay(50);
    buttonPressed = true;
    Serial.println("Кнопка нажата! Начинаем отсчет 5 секунд...");
  }
  
  if (buttonPressed && !trafficLightActive) {
    for(int i = 5; i > 0; i--) {
      Serial.print("До начала переключения: ");
      Serial.print(i);
      Serial.println(" сек");
      delay(1000);
    }
    
    trafficLightActive = true;
    buttonPressed = false;
    runTrafficLightCycle();
  }
  
  lastButtonState = currentButtonState;
  delay(10);
}

void runTrafficLightCycle() {
  Serial.println("=== НАЧАЛО ЦИКЛА СВЕТОФОРА ===");
  
  // Зеленый мигает последние 3 секунды
  Serial.println("Зеленый: 7 сек");
  delay(4000);
  
  for(int i = 3; i > 0; i--) {
    Serial.print("Зеленый мигает: ");
    Serial.print(i);
    Serial.println(" сек");
    digitalWrite(greenLed, !digitalRead(greenLed));
    delay(500);
    digitalWrite(greenLed, !digitalRead(greenLed));
    delay(500);
  }
  
  // Выключаем зеленый, включаем желтый
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, HIGH);
  Serial.println("Желтый: 1 сек");
  delay(1000);
  
  // Выключаем желтый, включаем красный
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, HIGH);
  Serial.println("Красный: 5 сек");
  delay(4000);
  
  // Последняя секунда красного + желтый
  digitalWrite(yellowLed, HIGH);
  Serial.println("Красный + Желтый: 1 сек");
  delay(1000);
  
  // Выключаем все, включаем зеленый
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, HIGH);
  
  Serial.println("=== КОНЕЦ ЦИКЛА СВЕТОФОРА ===");
  Serial.println("Ожидание следующего нажатия кнопки...");
  
  trafficLightActive = false;
}