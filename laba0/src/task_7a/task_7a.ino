const int GREEN_PIN = 9;
const int YELLOW_PIN = 10;
const int RED_PIN = 11;

const int GREEN_TIME = 10;
const int RED_TIME = 8;

void setup() {
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
}

void loop() {
  // Зеленый горит
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  
  for(int i = GREEN_TIME - 3; i > 0; i--) {
    delay(1000);
  }
  
  // Зеленый мигает
  for(int i = 3; i > 0; i--) {
    digitalWrite(GREEN_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_PIN, LOW);
    delay(500);
  }
  
  // Желтый
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
  delay(1000);
  
  // Красный горит
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, HIGH);
  
  for(int i = RED_TIME - 1; i > 0; i--) {
    delay(1000);
  }
  
  // Красный + Желтый
  digitalWrite(YELLOW_PIN, HIGH);
  delay(1000);
  
  // Переход к зеленому
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
}

}