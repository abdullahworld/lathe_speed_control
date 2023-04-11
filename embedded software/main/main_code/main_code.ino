
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <stdint.h>

// User controls
#define R_POT A1
#define AUTO 8    // S3
#define BUT_CCW 7 // S2
#define BUT_CW 5  // S1

// Motor controls
#define EN 3
#define PHASE 4

typedef void (*task_function_p)(void);

typedef struct
{
  uint16_t period;
  uint16_t elapsed_time;
  task_function_p task_function;
  const char *name;
} task_info_t;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

static task_info_t[] =
    {{.task_function = read_buttons, .period = 100, .elasped_time = 0, .name = "Read buttons"} {.task_function = get_current_speed, .period = 100, .elasped_time = 0, .name = "Get Lathe speed"} {.task_function = update_motors, .period = 100, .elasped_time = 0, .name = "Update the motors"}};

void update_motors(void)
{
  return;
}

void get_current_speed(void)
{
  return;
}

void read_buttons(void)
{
  int auto_val = digitalRead(AUTO);
  int cw_val = digitalRead(BUT_CW);
  int ccw_val = digitalRead(BUT_CCW);
}

void setup()
{
  Serial.begin(9600);
  lcd.init();

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System starting");
  delay(1000);
  lcd.clear();

  // configuring the buttons.
  pinMode(AUTO, INPUT_PULLUP);
  pinMode(BUT_CCW, INPUT_PULLUP);
  pinMode(BUT_CW, INPUT_PULLUP);

  pinMode(EN, OUTPUT);
  pinMode(PHASE, OUTPUT);
}

void loop()
{
  Serial.print("Auto Val: ");
  Serial.println(auto_val);

  Serial.print("CW Val: ");
  Serial.println(cw_val);

  Serial.print("CCW Val: ");
  Serial.println(ccw_val);
  digitalWrite(PHASE, HIGH);
  //  delay(2000);
  //  digitalWrite(PHASE, LOW);
}
