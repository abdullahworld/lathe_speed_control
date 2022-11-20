const int led_pin = PB5;

//Counter and compare values
const uint16_t t1_load = 0;
const uint16_t t1_comp = 0;


void setup (){
  Serial.begin(9600);
  DDRB |= (1 << led_pin);


  cli();
  TCCR1A = 0;

  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);

  TCNT1 = t1_load;
  OCR1A = t1_comp;

  TIMSK1 = (1 << OCIE1A);

  sei();
  
}


void loop() {
  
//  PORTB ^= (1 << led_pin);
//  delay(500);
 
}

ISR(TIMER1_COMPA_vect)
{
  TCNT1 = t1_load;
  Serial.println("hi");

}
