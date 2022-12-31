

#include <LiquidCrystal_I2C.h>
#include <Wire.h>


//User controls
#define R_POT A1
#define AUTO 8 //S3
#define BUT_CCW 7 //S2
#define BUT_CW 5 //S1

//Motor controls
#define EN 3
#define PHASE 4
#define SLEEP 10
#define MODE 6

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int r_pot;


void setup() {
  Serial.begin(9600);
  lcd.init();

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System starting");
  delay(1000);
  lcd.clear();
  
  //configuring the buttons.
  pinMode(AUTO, INPUT_PULLUP);
  pinMode(BUT_CCW, INPUT_PULLUP);
  pinMode(BUT_CW, INPUT_PULLUP);

  //configuring h-bridge driver circutry.  
  pinMode(PHASE, OUTPUT);
  pinMode(SLEEP, OUTPUT);
  pinMode(MODE, OUTPUT);
  
  digitalWrite(MODE, HIGH);
  digitalWrite(SLEEP, HIGH);
  digitalWrite(PHASE, LOW);

  pinMode(EN, OUTPUT);
  analogWrite(EN, 20);
//  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
//  TCCR2B = _BV(CS21);
//  OCR2A = 180;
//  OCR2B = 50;
  interrupts();

  

}

void loop() {
  // put your main code here, to run repeatedly:

}
