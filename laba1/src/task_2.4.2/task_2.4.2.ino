const uint8_t BUTTON_MASK = (1 << PD2) | (1 << PD3) | (1 << PD4);
const uint8_t LED_MASK = (1 << PB3) | (1 << PB4) | (1 << PB5);

int findFirstSetBitLoop(uint8_t value) {
  if (value == 0) return -1;  // Ни один бит не установлен
  
  for (int i = 0; i < 8; i++) {
    if (value & (1 << i)) {
      return i;  // Нашли первый установленный бит (с младшего)
    }
  }
  return -1;
}

void setup() {
  DDRB = 0b00111000;
  DDRD = 0b00000000;
  PORTD = 0b00011100;
  PORTB = 0b00000000;
  Serial.begin(9600);
}

void loop() {
  uint8_t val = PIND;
  uint8_t pressed_buttons = ~val & BUTTON_MASK;
  uint8_t led_states = pressed_buttons << 1;
  
  PORTB = (PORTB & ~LED_MASK) | (led_states & LED_MASK);
  
  int firstBit = findFirstSetBitLoop(pressed_buttons);
  
  Serial.print("Нажатые кнопки: ");
  for (int i = 7; i >= 0; i--) Serial.print((pressed_buttons >> i) & 1);
  Serial.print(" | Первый бит (цикл): ");
  if (firstBit != -1) {
    Serial.print("D");
    Serial.print(firstBit);
  } else {
    Serial.print("нет");
  }
  Serial.println();
  
  delay(500);
}