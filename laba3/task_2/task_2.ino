const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);

  // 1. Отключаем таймер, чтобы настроить его
  TCCR1A = 0;
  TCCR1B = 0;
  // 2. Устанавливаем режим CТС (WGM12=1, WGM13=0) 
  TCCR1B |= (1 << WGM12);
  // 3. Устанавливаеи значение TOP (OCR1A = 2499) 
  OCR1A = 2499;
  // 4. Включаем прерывание по совпадению A (OCR1A) 
  TIMSK1 |= (1 << OCIE1A);
  // 5. Запускаем таймер с предделителем 64 (CS12=1, CS11=1, CS10=0) 
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // Включаем глобальные прерывания 
  sei();
}
  // Обработчик прерывания по совпадению A Timer1
ISR(TIMER1_COMPA_vect) {
  // Инвертируем состояние светодиода каждые 10 мс 
  static bool ledState = false; 
  ledState = !ledState; 
  digitalWrite(ledPin, ledState);
}
void loop() {
}