const int buttonPin = 2;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

bool buttonPressed = false;
bool lastButtonState = HIGH;
bool trafficLightActive = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  
  setGreen();
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

void setGreen() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
}

void setYellow() {
  // Желтый = Красный (255) + Зеленый (150)
  analogWrite(redPin, 255);
  analogWrite(greenPin, 150);
  analogWrite(bluePin, 0);
}

void setRed() {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void setRedYellow() {
  // Красный + Желтый = Красный (255) + Зеленый (75) - более оранжевый оттенок
  analogWrite(redPin, 255);
  analogWrite(greenPin, 75);
  analogWrite(bluePin, 0);
}

void setOff() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void runTrafficLightCycle() {
  Serial.println("=== НАЧАЛО ЦИКЛА СВЕТОФОРА ===");
  
  // Зеленый горит 7 секунд
  setGreen();
  Serial.println("Зеленый: 7 сек");
  delay(4000);
  
  // Зеленый мигает 3 секунды
  for(int i = 3; i > 0; i--) {
    Serial.print("Зеленый мигает: ");
    Serial.print(i);
    Serial.println(" сек");
    setGreen();
    delay(500);
    setOff();
    delay(500);
  }
  
  setYellow();
  Serial.println("Желтый: 1 сек");
  delay(1000);
  
  setRed();
  Serial.println("Красный: 5 сек");
  delay(4000);
  
  setRedYellow();
  Serial.println("Красный + Желтый: 1 сек");
  delay(1000);
  
  setGreen();
  Serial.println("=== КОНЕЦ ЦИКЛА СВЕТОФОРА ===");
  Serial.println("Ожидание следующего нажатия кнопки...");
  
  trafficLightActive = false;
}