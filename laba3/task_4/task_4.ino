#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= LCD ==================
LiquidCrystal_I2C lcd(0x27, 16, 2); // Адрес дисплея и размер 16x2

// ================= Клавиатура ==================
const int rowPins[4] = {8, 7, 6, 5};  // Строки
const int colPins[4] = {11, 10, 9, 4}; // Столбцы
const int KEY_INT_PIN = 2;             // D2 — внешний прерывание INT0

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// ================= Глобальные переменные ==================
volatile unsigned long timerCount = 0; // Счётчик, увеличивается каждые 500 мс
volatile bool scanFlag = false;        // Флаг: нужно сканировать клавиатуру
bool keyPressed = false;
char lastKey = 0;

// ================== Настройка таймера 1 ==================
void setupTimer1() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);             // Режим CTC
  OCR1A = 7812;                       // Прерывание каждые 0.5 сек при 16 МГц / 1024
  TCCR1B |= (1 << CS12) | (1 << CS10); // Предделитель 1024
  TIMSK1 |= (1 << OCIE1A);            // Разрешаем прерывание
}

// Прерывание таймера — обновляет LCD
ISR(TIMER1_COMPA_vect) {
  timerCount++;
}

// ================== Внешнее прерывание ==================
void startScan() {
  scanFlag = true;  // Установим флаг для сканирования клавиатуры
}

// ================== Настройка клавиатуры ==================
void setupKeyboardPins() {
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH); // Все строки в HIGH (неактивные)
  }

  for (int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP); // Столбцы с подтяжкой
  }
}

// ================== Функция сканирования клавиатуры ==================
char scanKeyboard() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(rowPins[i], LOW); // Активируем одну строку
    delayMicroseconds(500);        // Короткая стабилизация
    for (int j = 0; j < 4; j++) {
      if (digitalRead(colPins[j]) == LOW) { // Клавиша нажата
        digitalWrite(rowPins[i], HIGH);
        return keys[i][j];
      }
    }
    digitalWrite(rowPins[i], HIGH);
  }
  return 0; // Нет нажатий
}

// ================== Основная программа ==================
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  setupKeyboardPins();
  setupTimer1();

  attachInterrupt(digitalPinToInterrupt(KEY_INT_PIN), startScan, FALLING);
  sei(); // Включаем глобальные прерывания

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
}

void loop() {
  static unsigned long lastDisplayUpdate = 0;

  // ---- 1. Обновление LCD каждые 500 мс (из Timer1) ----
  if (timerCount > lastDisplayUpdate) {
    lcd.setCursor(0, 0);
    lcd.print("Count: ");
    lcd.print(timerCount);
    lcd.print("   "); // Стираем хвосты
    lastDisplayUpdate = timerCount;
  }

  // ---- 2. Проверка флага сканирования от EXT INT0 ----
  if (scanFlag) {
    char pressedKey = scanKeyboard();
    if (pressedKey != 0) {
      keyPressed = true;
      lastKey = pressedKey;

      Serial.print("Key detected: ");
      Serial.println(pressedKey);

      lcd.setCursor(0, 1);
      lcd.print("Key: ");
      lcd.print(pressedKey);
      lcd.print("   ");

      // Пример реакции: сброс счётчика по клавише 'A'
      if (pressedKey == 'A') {
        timerCount = 0;
        lcd.setCursor(0, 0);
        lcd.print("Count: 0     ");
      }
    }
    scanFlag = false;
  }
}
