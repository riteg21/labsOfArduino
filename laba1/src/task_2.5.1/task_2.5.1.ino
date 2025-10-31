const uint8_t BUTTON_MASK = (1 << PD2) | (1 << PD3) | (1 << PD4);
const uint8_t LED_MASK = (1 << PB3) | (1 << PB4) | (1 << PB5);

int findFirstSetBitCTZ(uint8_t value) {
  if (value == 0) return -1;
  return __builtin_ctz(value);  
}

int findFirstSetBitFFS(uint8_t value) {
  int result = __builtin_ffs(value);  // Индекс с 1
  return result > 0 ? result - 1 : -1;  // Индекс с 0
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
  
  int firstBitCTZ = findFirstSetBitCTZ(pressed_buttons);
  int firstBitFFS = findFirstSetBitFFS(pressed_buttons);
  
  Serial.print("Нажатые кнопки: ");
  for (int i = 7; i >= 0; i--) Serial.print((pressed_buttons >> i) & 1);
  Serial.print(" | CTZ: ");
  if (firstBitCTZ != -1) Serial.print("D" + String(firstBitCTZ));
  else Serial.print("нет");
  Serial.print(" | FFS: ");
  if (firstBitFFS != -1) Serial.print("D" + String(firstBitFFS));
  else Serial.print("нет");
  Serial.println();
  
  delay(500);
}