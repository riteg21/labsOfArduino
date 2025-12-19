#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int RTC_RST = 8, RTC_DAT = 7, RTC_CLK = 6;
DS1302 rtc(RTC_RST, RTC_DAT, RTC_CLK);
QueueHandle_t luxQueue;

float calculateLuxFromADC(int adcValue) {
  float x = (float)adcValue; // Преобразуем в float для расчетов
  float y;
  
  // Кусочно-заданная полиномиальная функция
  if (x >= 214) {
    // y = -8E-08*x^5 + 7E-05*x^4 - 0.0213*x^3 + 3.0737*x^2 - 205.9*x + 6014.8
    y = -8e-08 * pow(x, 5) + 
         7e-05 * pow(x, 4) - 
         0.0213 * pow(x, 3) + 
         3.0737 * pow(x, 2) - 
         205.9 * x + 
         6014.8;
  } else {
    // y = -7E-06*x^3 + 0.013*x^2 - 8.4151*x + 1832.8
    y = -7e-06 * pow(x, 3) + 
         0.013 * pow(x, 2) - 
         8.4151 * x + 
         1832.8;
  }
  
  // Защита от некорректных значений
  if (y < 0) y = 0;
  if (y > 4000) y = 4000;
  
  return y;
}

void TaskMeasure(void *pv) {
  (void) pv;
  float lux;
  
  for (;;) {
    int adc = analogRead(A0);
    lux = calculateLuxFromADC(adc);
    
    // Отправляем в очередь
    if (xQueueSend(luxQueue, &lux, 10 / portTICK_PERIOD_MS) != pdPASS) {
      // Если очередь полна, пропускаем измерение
    }
    
    // Отладка в Serial
    Serial.print("ADC: ");
    Serial.print(adc);
    Serial.print(" -> Lux: ");
    Serial.println(lux, 1);
    
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void TaskDisplay(void *pv) {
  (void) pv;
  float lux = 0;
  
  // Инициализация LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Luxmeter RTOS");
  
  // Инициализация RTC
  rtc.halt(false);
  rtc.writeProtect(false);
  // При первом запуске - снимаем комментарий для инициализации времени
  // Потом - комментируем обратно
  //Time t(2025, 12, 5, 15, 48, 0, Time::kMonday);
  //rtc.time(t);
  
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  lcd.clear();
  
  for (;;) {
    if (xQueueReceive(luxQueue, &lux, 100 / portTICK_PERIOD_MS) == pdPASS) {
      // Строка 1: Освещенность
      lcd.setCursor(0, 0);
      lcd.print("Lux: ");
      lcd.print(lux, 0);
      lcd.print(" lx  ");
      
      // Строка 2: Время от RTC
      Time now = rtc.time();
      lcd.setCursor(0, 1);
      if (now.hr < 10) lcd.print("0");
      lcd.print(now.hr);
      lcd.print(":");
      if (now.min < 10) lcd.print("0");
      lcd.print(now.min);
      lcd.print(":");
      if (now.sec < 10) lcd.print("0");
      lcd.print(now.sec);
      lcd.print("    ");
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  luxQueue = xQueueCreate(3, sizeof(float));
  
  xTaskCreate(TaskMeasure, "Measure", 80, NULL, 1, NULL);
  xTaskCreate(TaskDisplay, "Display", 150, NULL, 1, NULL);
  
  vTaskStartScheduler();
}

void loop() {}