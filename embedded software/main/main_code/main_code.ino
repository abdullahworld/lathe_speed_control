
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define R_POT A1
#define AUTO 8 //S3
#define BUT_CCW 7 //S2
#define BUT_CW 6 //S1



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
  
  //configuring the buttons.
  pinMode(AUTO, INPUT_PULLUP);
  pinMode(BUT_CCW, INPUT_PULLUP);
  pinMode(BUT_CW, INPUT_PULLUP);
}

void loop()
{
  r_pot = analogRead(R_POT);

  Serial.println(r_pot);
  lcd.setCursor(0,0);
  lcd.print("RPOT: ");
  lcd.setCursor(6,0);
  lcd.print(r_pot);

  int auto_val = digitalRead(AUTO);
  int cw_val = digitalRead(BUT_CW);
  int ccw_val = digitalRead(BUT_CCW);

  Serial.print("Auto Val: ");
  Serial.println(auto_val);

  Serial.print("CW Val: ");
  Serial.println(cw_val);

  Serial.print("CCW Val: ");
  Serial.println(ccw_val);



  
  
  delay(100);
}