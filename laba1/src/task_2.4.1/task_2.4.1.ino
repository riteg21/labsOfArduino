void setup() {
  bitSet(DDRB, PB3); bitSet(DDRB, PB4); bitSet(DDRB, PB5);
  bitClear(DDRD, PD2); bitClear(DDRD, PD3); bitClear(DDRD, PD4);
  bitSet(PORTD, PD2); bitSet(PORTD, PD3); bitSet(PORTD, PD4);
  PORTB = 0;
  Serial.begin(9600);
}

void loop() {
  uint8_t led_states = 0;

  if (!bitRead(PIND, PD2)) bitSet(led_states, PB3);  // Кнопка 2 → LED 11
  if (!bitRead(PIND, PD3)) bitSet(led_states, PB4);  // Кнопка 3 → LED 12
  if (!bitRead(PIND, PD4)) bitSet(led_states, PB5);  // Кнопка 4 → LED 13
  
  Serial.print("Байт нажатых кнопок: ");
  for (int i = 7; i >= 0; i--) {
    Serial.print(bitRead(led_states, i));
  }
  Serial.println();
  
  for (int i = 0; i < 8; i++) {
    if (bitRead(led_states, i)) {
      bitSet(PORTB, i);
    } else {
      bitClear(PORTB, i);
    }
  }
  
  delay(200);
}