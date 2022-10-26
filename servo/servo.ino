#include <Servo.h>

#define SERVO 3 // Porta Digital 6 PWM

Servo s; // Variável Servo
int pos; // Posição Servo
int vel=50;
int delei=10;
void setup ()
{
  s.attach(SERVO);
  Serial.begin(9600);
  s.write(0); // Inicia motor posição zero
}

void loop()
{
  for(pos = 0; pos <= 91; pos++)
  {
    s.write(pos);
  delay(vel);
  }
delay(delei);
  for(pos = 90; pos >= 0; pos--)
  {
    s.write(pos);
    delay(vel);
  }
delay(delei);
}
