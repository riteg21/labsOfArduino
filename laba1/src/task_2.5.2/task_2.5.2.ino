const uint8_t BUTTON_MASK = (1 << PD2) | (1 << PD3) | (1 << PD4);
const uint8_t LED_MASK = (1 << PB3) | (1 << PB4) | (1 << PB5);

uint32_t lastProcessTime = 0;
uint8_t lastStableState = 0;

void setup() {
  DDRB = 0b00111000;
  DDRD = 0b00000000;
  PORTD = 0b00011100;
  PORTB = 0b00000000;
  Serial.begin(9600);
}

void loop() {
  uint32_t currentTime = millis();
  
  // Гарантируем частоту 5 Гц (каждые 200ms)
  if (currentTime - lastProcessTime >= 200) {
    
    uint8_t currentState = PIND;
    uint8_t pressed_buttons = ~currentState & BUTTON_MASK;
    
    // Простой антидребезг - обновляем только если состояние стабильно
    if (pressed_buttons == lastStableState) {
      // Состояние стабильно - обрабатываем
      uint8_t led_states = pressed_buttons << 1;
      PORTB = (PORTB & ~LED_MASK) | (led_states & LED_MASK);
      
      // Вывод в терминал
      Serial.print("Нажатые: ");
      for (int i = 7; i >= 0; i--) {
        Serial.print((pressed_buttons >> i) & 1);
      }
      Serial.println();
    }
    
    lastStableState = pressed_buttons;
    lastProcessTime = currentTime;
  }
}