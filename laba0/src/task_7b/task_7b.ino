const int greenLed = 11;
const int yellowLed = 12;
const int redLed = 13;
const int greenTime = 10;
const int redTime = 8;

void setup() {
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
}

void loop() {
  Serial.println("=== ЗЕЛЕНЫЙ ===");
  digitalWrite(greenLed, HIGH);
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, LOW);
  
  for(int i = greenTime; i > 0; i--) {
    Serial.print("Зеленый: ");
    Serial.print(i);
    Serial.println(" сек");
    
    if(i <= 4) {
      for(int j = 0; j < 10; j++) {
        digitalWrite(greenLed, !digitalRead(greenLed));
        delay(100);
      }
    } else {
      delay(1000);
    }
  }

  Serial.println("=== ЖЕЛТЫЙ ===");
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, HIGH);
  
  for(int i = 1; i > 0; i--) {
    Serial.print("Желтый: ");
    Serial.print(i);
    Serial.println(" сек");
    delay(1000);
  }

  Serial.println("=== КРАСНЫЙ ===");
  digitalWrite(yellowLed, LOW);
  digitalWrite(redLed, HIGH);
  
  for(int i = redTime; i > 0; i--) {
    Serial.print("Красный: ");
    Serial.print(i);
    Serial.println(" сек");
    
    if(i == 1) {
      digitalWrite(yellowLed, HIGH);
      Serial.println("Красный + Желтый");
    }
    
    delay(1000);
  }

  Serial.println("=== ПЕРЕКЛЮЧЕНИЕ ===");
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  delay(1000);
}