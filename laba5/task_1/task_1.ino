int photoPin = A0;    // Пин подключения фоторезистора
int adcValue;         // Сырое значение АЦП (0-1023)
float voltage;        // Напряжение (0-5V)
int resistance;       // Сопротивление фоторезистора

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Сырое значение АЦП
  adcValue = analogRead(photoPin);
  
  // Расчет напряжения
  voltage = adcValue * (5.0 / 1023.0);
  
  // Расчет сопротивления фоторезистора
  resistance = 10000 * (5.0 / voltage - 1); // для резистора 10 Ом
  
  Serial.print("АЦП: ");
  Serial.print(adcValue);
  Serial.print(" | Напряжение: ");
  Serial.print(voltage);
  Serial.print("V | Сопротивление: ");
  Serial.print(resistance);
  Serial.println(" Ом");
  
  delay(1000);
}