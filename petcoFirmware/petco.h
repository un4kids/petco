#ifndef _PETCO_
#define _PETCO_

#include "Arduino.h"
#include "Petco_Pinout.h"
#include "Petco_Constants.h"
#include <Servo.h>

// Servo object initialization
extern Servo servo_X;
extern Servo servo_Y;

// Servo position parameters
extern int angle;

// Distance measurement buffers and values
extern int leftDistance;
extern int rightDistance;
extern int distanceDelta;
extern int realDistance;

// LED control methods
#define LEDon() digitalWrite(LED, LOW)
#define LEDoff() digitalWrite(LED, HIGH)

// Ultrasonic distance measurement methods
#define getDistanceL() getDistance(TRIG_L, ECHO_L)
#define getDistanceR() getDistance(TRIG_R, ECHO_R)

// Declarations
void initialization();

int getDistance(int TRIG, int ECHO);
void measureDistance();
void printDistance();

void forward();
void reverse();
void left();
void right();
void stop();

void escape();

void turnHead();
void turnHeadNew(int angle_X = SERVO_X_CENTER, int angle_Y = SERVO_Y_CENTER);

double readVcc(double result = 1100.0);
void indicateVoltage(double threshold = 4200.0);

#endif
