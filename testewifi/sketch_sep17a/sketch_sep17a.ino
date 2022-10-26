#include <ESP8266_Lib.h>
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include<AFMotor.h>

SoftwareSerial EspSerial(0, 1); // RX, TX
#define ESP8266_BAUD 9600
ESP8266 wifi(&Serial);
char auth[] = "281f8675c5cc4c55a4d259ef7518d3**";

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR12_1KHZ);
AF_DCMotor motor4(4, MOTOR12_1KHZ);

void setup()
{
  // Set console baud rate
  Serial.begin(9600);
  delay(10);
  Serial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, "Tonio_5G", "xpto3500"); // wifi username and password
}
BLYNK_WRITE(V1)
{
  int x = param[0].asInt();
  int y = param[1].asInt();
  // Do something with x and y

  if (y > 220)
    forward();
  else if (y < 35)
    backward();
  else if (x > 220)
    right();
  else if (x < 35)
    left();
  else
    Stop();
}
void loop()
{
  Blynk.run();
}


void forward()
{
  /*motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  motor3.setSpeed(255);
  motor3.run(FORWARD);
  motor4.setSpeed(255);
  motor4.run(FORWARD);*/
}

void backward()
{
  /*motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
  motor3.setSpeed(255);
  motor3.run(BACKWARD);
  motor4.setSpeed(255);
  motor4.run(BACKWARD);*/
}

void left()
{
 /* motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  motor3.setSpeed(0);
  motor3.run(RELEASE);
  motor4.setSpeed(0);
  motor4.run(RELEASE);*/
}

void right()
{
  /*motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  motor3.setSpeed(255);
  motor3.run(FORWARD);
  motor4.setSpeed(255);
  motor4.run(FORWARD);*/
}

void Stop()
{/*
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  motor3.setSpeed(0);
  motor3.run(RELEASE);
  motor4.setSpeed(0);
  motor4.run(RELEASE);
*/}
