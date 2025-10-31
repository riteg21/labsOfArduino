const int R_PIN = 9;
const int G_PIN = 10;
const int B_PIN = 11;
const int POT_PIN = A0;

int hue = 0;  // 0..359

void setRGB(int r, int g, int b) {
  analogWrite(R_PIN, r);
  analogWrite(G_PIN, g);
  analogWrite(B_PIN, b);
}

void hsvToRgb(int hue, int &red, int &green, int &blue) {
  float huePosition = hue / 60.0;
  int sector = (int)floor(huePosition) % 6;
  float fraction = huePosition - sector;

  int value = 255;  // Фиксированная максимальная яркость
  int decreasing = (int)(value * (1 - fraction));
  int increasing = (int)(value * fraction);

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
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(POT_PIN);
  hue = map(potValue, 0, 1023, 0, 359);

  int r, g, b;
  hsvToRgb(hue, r, g, b);
  setRGB(r, g, b);

  Serial.print("Hue=");
  Serial.print(hue);
  Serial.print(" | RGB=(");
  Serial.print(r);
  Serial.print(",");
  Serial.print(g);
  Serial.print(",");
  Serial.print(b);
  Serial.println(")");

  delay(100);
}