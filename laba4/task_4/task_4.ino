#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const long updateInterval = 1000;
unsigned long previousUpdateTime = 0;
float temperature = 25.0;
int mode = 1;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("System Initialized");
  lcd.setCursor(0, 1);
  lcd.print("Mode: 1");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousUpdateTime >= updateInterval) {
    previousUpdateTime = currentMillis;

    temperature += 0.1;
    if (temperature > 30.0) {
      temperature = 20.0;
    }

    mode = (mode % 3) + 1;

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("C  ");

    lcd.setCursor(0, 1);
    lcd.print("Mode: ");
    if (mode == 1) {
      lcd.print("Manual  ");
    } else if (mode == 2) {
      lcd.print("Auto    ");
    } else {
      lcd.print("Sleep   ");
    }
  }
}
