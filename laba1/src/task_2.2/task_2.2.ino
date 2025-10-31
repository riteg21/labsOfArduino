void setup() {
  DDRB = 0b00111000;
  DDRD = 0b00000000;
  PORTD = 0b00011100;
  PORTB = 0b00000000;
  Serial.begin(9600);
}
const uint8_t BUTTON_MASK = (1 << PD2) | (1 << PD3) | (1 << PD4); 
void loop() {
  uint8_t val = PIND;
  uint8_t pressed = ~val & BUTTON_MASK;
  Serial.print("Позиции нажатых кнопок: ");
  for (uint8_t n = 0; n < 8; ++n) {
  if (pressed & (1 << n)) Serial.print(n, " ");
  }
  Serial.println();
  delay(500);
}