#ifndef _PETCO_CONSTANTS_
#define _PETCO_CONSTANTS_

// Serial
#define SERIAL_BAUD 9600	// Serial(Debug) interface baud rate

// Servo constants
#define SERVO_X_CENTER 90	// 90 -> straight 0 -> full right
#define SERVO_X_STEP 10		// turn step
#define MIN_X_ANGLE 20		// maximum right position angle
#define MAX_X_ANGLE 160		// maximum left position angle

#define SERVO_Y_CENTER 75	// 75 -> straight ahead
#define SERVO_Y_STEP 10		// turn step
#define MIN_Y_ANGLE 75		// maximum down position angle
#define MAX_Y_ANGLE 85		// maximum up position angle

// Wheel speeds
#define SPEEDL 140
#define SPEEDR 140

#endif
