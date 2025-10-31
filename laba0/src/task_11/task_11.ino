const int R_PIN = 9;
const int G_PIN = 10;
const int B_PIN = 11;

const int ENCODER_PIN_A = 2;
const int ENCODER_PIN_B = 3;
const int BUTTON_PIN = 4;

int hue = 0;          // 0..359
int brightness = 255; // 0..255 (Value)

// Для энкодера
int lastA = HIGH;
int lastB = HIGH;

void setRGB(int r, int g, int b) {
  analogWrite(R_PIN, r);
  analogWrite(G_PIN, g);
  analogWrite(B_PIN, b);
}

void hsvToRgb(int hue, int brightness, int &red, int &green, int &blue) {
  // Делим оттенок на 60°, чтобы понять сектор (0..5)
  float huePosition = hue / 60.0;
  int sector = (int)floor(huePosition) % 6;
  float fraction = huePosition - sector; // дробная часть для плавного перехода

  // Промежуточные значения для плавного перехода между секторами
  int value = brightness; // максимальное значение (V)
  int decreasing = (int)(value * (1 - fraction));  // спадающий канал
  int increasing = (int)(value * fraction);        // возрастающий канал

  switch (sector) {
    case 0: red = value;      green = increasing;  blue = 0;        break;
    case 1: red = decreasing; green = value;       blue = 0;        break;
    case 2: red = 0;          green = value;       blue = increasing; break;
    case 3: red = 0;          green = decreasing;  blue = value;    break;
    case 4: red = increasing; green = 0;           blue = value;    break;
    case 5: red = value;      green = 0;           blue = decreasing; break;
  }
}



void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lastA = digitalRead(ENCODER_PIN_A);
  lastB = digitalRead(ENCODER_PIN_B);

  Serial.begin(9600);
}

void loop() {
  int A = digitalRead(ENCODER_PIN_A);
  int B = digitalRead(ENCODER_PIN_B);
  if (A != lastA) { // Изменился канал A → шаг энкодера
    if (A == LOW) { 
      // Определяем направление по каналу B
      if (B == HIGH) {
        encoderTurn(+1); // вправо
      } else {
        encoderTurn(-1); // влево
      }
    }
  }

  lastA = A;
  lastB = B;

  int r, g, b;
  hsvToRgb(hue, brightness, r, g, b);
  setRGB(r, g, b);
}

// Обработка шага энкодера
void encoderTurn(int dir) {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    // Кнопка нажата → меняем яркость
    brightness += dir * 5;
    if (brightness < 0) brightness = 0;
    if (brightness > 255) brightness = 255;

    Serial.print("Поворот: ");
    Serial.print(dir > 0 ? "вправо" : "влево");
    Serial.print(" | Режим: ЯРКОСТЬ | Яркость=");
    Serial.println(brightness);

  } else {
    // Кнопка не нажата → меняем цвет (Hue)
    hue += dir * 3;
    if (hue < 0) hue += 360;
    if (hue >= 360) hue -= 360;

    Serial.print("Поворот: ");
    Serial.print(dir > 0 ? "вправо" : "влево");
    Serial.print(" | Режим: ЦВЕТ | Hue=");
    Serial.println(hue);
  }
}