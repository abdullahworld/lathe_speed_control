
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
#define PH 4

// Revolution pulse
#define REV_PULSE 2

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

static uint32_t prev_tech_counter = 0;
static uint32_t tach_counter = 0;
static uint32_t prev_time = 0;
static int32_t current_speed_rpm;


typedef void (*task_function_p)(void);

typedef struct
{
  uint16_t period;
  uint16_t elapsed_time;
  task_function_p task_function;
  const char *name;
} task_info_t;

static void update_motors(void);
static void update_display(void);
static void software_pwm(void);
void read_buttons(void);
static void get_current_speed(void);


task_info_t periodic_tasks[] =
{
  {.period = 20, .elapsed_time = 0, .task_function = read_buttons, .name = "Read buttons"},
  {.period = 2, .elapsed_time = 0, .task_function = get_current_speed, .name = "Get Lathe speed"},
  {.period = 2, .elapsed_time = 0, .task_function = software_pwm, .name = "Update the motor position"},
  {.period = 100, .elapsed_time = 0, .task_function = update_display, .name = "Update LCD display"},
};


// static void update_motors(void)
// {

// }


void software_pwm()
{
  digitalWrite(EN, HIGH);
  digitalWrite(PH, LOW);
  delayMicroseconds(5000);
  digitalWrite(EN, LOW);
  digitalWrite(PH, HIGH);
}

static void increment_tach_counter(void)
{
  tach_counter++;
}


static void get_current_speed(void)
{
  // rpm is equal to the number of ticks divided by time then convert to rpm.
  uint32_t delta_tach_counter = tach_counter - prev_tech_counter;
  uint32_t current_time = millis();
  uint32_t delta_time = current_time - prev_time;
  prev_time = current_time;
  // get speed in ticks per millisecond
  int32_t current_speed = delta_tach_counter / delta_time;
  current_speed_rpm = current_speed * 3600000;
  
  current_speed_rpm;
}


void read_buttons(void)
{
  int auto_val = digitalRead(AUTO);
  int cw_val = digitalRead(BUT_CW);
  int ccw_val = digitalRead(BUT_CCW);
}


static void update_display(void)
{
  return;
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

  // configure h-bridge PWM signals
  pinMode(EN, OUTPUT);
  pinMode(PH, OUTPUT);

  // configure the pulse from tachometer as in interrupt
  pinMode(REV_PULSE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(REV_PULSE), increment_tach_counter, FALLING);
}


void run_all_tasks(void)
{  
  uint32_t i;
  task_info_t *task_p;

  for (int i = 0; i < (sizeof(periodic_tasks) / sizeof(periodic_tasks[i])); i++)
  {
    task_p = &periodic_tasks[i];

    if (task_p->task_function != NULL)
    {
      task_p->elapsed_time++;
      if (task_p->elapsed_time >= task_p->period)
      {
        task_p->elapsed_time = 0;
        task_p->task_function();
      }
    }
  }
}


void loop()
{
  int i = 0;
  run_all_tasks();
  Serial.print("Tach counter: "); 
  Serial.println(tach_counter);
  Serial.print("Current speed (rpm): ");
  Serial.println(current_speed_rpm);

}
