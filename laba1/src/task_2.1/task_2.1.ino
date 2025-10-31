void setup() {
  DDRB = 0b00111000;
  DDRD = 0b00000000;
  PORTD = 0b00011100;
  PORTB = 0b00000000;
  Serial.begin(9600);
}

void loop() {
  uint8_t portState = PIND;
  
  for (int i = 7; i >= 0; i--) {
    Serial.print((portState >> i) & 1);
  }
  Serial.println();
  
  delay(200);
}