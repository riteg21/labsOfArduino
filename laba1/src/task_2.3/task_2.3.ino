const uint8_t BUTTON_MASK = (1 << PD2) | (1 << PD3) | (1 << PD4);
const uint8_t LED_MASK = (1 << PB3) | (1 << PB4) | (1 << PB5);

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
  uint8_t led_states = pressed_buttons << 1;  // Сдвиг битов 2,3,4 → 3,4,5
  
  Serial.print("Байт нажатых кнопок: ");
  for (int i = 7; i >= 0; i--) {
    Serial.print((led_states >> i) & 1);
  }
  Serial.println();
  
  // Запись в порт через маску
  PORTB = (PORTB & ~LED_MASK) | (led_states & LED_MASK);
  
  delay(200);
}