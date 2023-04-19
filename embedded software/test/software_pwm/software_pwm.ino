#include <stdint.h>


#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//User controls
#define R_POT A1
#define AUTO 8 //S3
#define BUT_CCW 7 //S2
#define BUT_CW 5 //S1

//Motor controls
#define INPUT_A 3
#define INPUT_B 4
#define P1_INT digitalPinToInterrupt(INPUT_A)
#define SLEEP 10
#define MODE 6

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int r_pot;


void setup() 
{
  Serial.begin(9600);
  lcd.init();

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System starting");
  delay(1000);
  lcd.clear();

  //configuring h-bridge driver circutry.  
  pinMode(EN, OUTPUT);
  pinMode(INPUT_B, OUTPUT);
}

void software_pwm()
{
  digitalWrite(EN, HIGH);
  digitalWrite(PH, LOW);
  delayMicroseconds(5000);
  digitalWrite(EN, LOW);
  digitalWrite(PH, HIGH);
  delayMicroseconds(5000);
}
 

void loop() 
{
  software_pwm();
}
