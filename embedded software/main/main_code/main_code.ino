
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
static int32_t current_speed_rpm_global;
static int32_t reference_speed_rpm;


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
static void speed_controller(void);
static void software_pwm(void);
void read_buttons(void);
static void get_current_speed(void);


task_info_t periodic_tasks[] =
{
  {.period = 20, .elapsed_time = 0, .task_function = read_buttons, .name = "Read buttons"},
  {.period = 2, .elapsed_time = 0, .task_function = speed_controller, .name = "Control Lathe speeed"},
  {.period = 100, .elapsed_time = 0, .task_function = update_display, .name = "Update LCD display"},
};


static void motor_clockwise()
{
  digitalWrite(EN, HIGH);
  digitalWrite(PH, LOW);
  delayMicroseconds(5000);
  digitalWrite(EN, LOW);
  digitalWrite(PH, HIGH);
}


static void motor_anticlockwise()
{
  digitalWrite(PH, LOW);
  digitalWrite(EN, HIGH);
  delayMicroseconds(5000);
  digitalWrite(PH, HIGH);
  digitalWrite(EN, LOW);
}


static void increment_tach_counter(void)
{
  tach_counter++;
}


int32_t calculate_speed(void)
{
    // rpm is equal to the number of ticks divided by time then convert to rpm.
  uint32_t delta_tach_counter = tach_counter - prev_tech_counter;
  uint32_t current_time = millis();
  uint32_t delta_time = current_time - prev_time;
  prev_time = current_time;
  // get speed in ticks per millisecond
  int32_t current_speed = delta_tach_counter / delta_time;
  int32_t current_speed_rpm = current_speed * 3600000;

  return current_speed_rpm;
}


static void speed_controller(void)
{
  current_speed_rpm_global = calculate_speed();
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


int32_t get_reference_speed(void)
{
  uint32_t current_time = 0; 
  int32_t current_speed_rpm = -1; //Set current speed to be an impossible value
  int32_t prev_speed_rpm = -2; //Set previous to be an impossible value

  uint32_t tol_percent = 0.01;
  
  // Get into a steady speed for at least 30 seconds
  while (current_time < 30)
  {
    delay(1000);
    current_speed_rpm = calculate_speed();
    if (current_speed_rpm != prev_speed_rpm + prev_speed_rpm * tol_percent || 
        current_speed_rpm != prev_speed_rpm - prev_speed_rpm * tol_percent )
    {
      current_time = 0;
    }
    current_speed_rpm = prev_speed_rpm; 
    current_time += 1;
  }

  Serial.println(current_speed_rpm);
  return current_speed_rpm;
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

  analogWrite(EN, 125);

  reference_speed_rpm = get_reference_speed();
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
  run_all_tasks();
}
