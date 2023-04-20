/*
/**
 * https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm
 * Read above to understand how to create a hardware pwm then use it to test reference 
 * speed code.
**/

#define EN 3

void setup()
{
    Serial.begin(9600);

    pinMode(EN, OUTPUT);
}

void loop()
{
    analogWrite(EN, 125);
    Serial.println("Hi");
}