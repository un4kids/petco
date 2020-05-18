#include "Arduino.h"
#include "Petco.h"

// Servo object initialization
Servo servo_X;
Servo servo_Y;

// Servo position parameters
int angle = SERVO_X_CENTER;

// Distance measurement buffers and values
int leftDistance = 0;
int rightDistance = 0;
int distanceDelta = 0;
int realDistance = 0;

// Initialization

void initialization() {
	Serial.begin(SERIAL_BAUD);
	Serial.println("=== Initialization ===");
	Serial.println("=== serial port ready ===");

	servo_X.attach(SERVO_X);
	servo_X.write(SERVO_X_CENTER);
	delay(50);
	Serial.println("=== servo X ready ===");

	servo_Y.attach(SERVO_Y);
	servo_Y.write(SERVO_Y_CENTER);
	delay(50);
	Serial.println("=== servo Y ready ===");

	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	Serial.println("=== LED ready ===");

	pinMode(ECHO_L, INPUT);
	pinMode(TRIG_L, OUTPUT);
	Serial.println("=== sensor L ready ===");

	pinMode(ECHO_R, INPUT);
	pinMode(TRIG_R, OUTPUT);
	Serial.println("=== sensor R ready ===");

	pinMode(IN1, OUTPUT);	// Right motor + red wire
	pinMode(IN2, OUTPUT);	// Right motor - black wire
	Serial.println("=== motor R ready ===");
	pinMode(IN3, OUTPUT);	// Left motor + red wire
	pinMode(IN4, OUTPUT);	// Left motor - black wire
	Serial.println("=== motor L ready ===");
	stop();

	Serial.println("=== Ready ===");
}

// Ultrasonic distance measurement methods

int getDistance(int TRIG, int ECHO) {
	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIG, LOW);
	float Fdistance = pulseInLong(ECHO, HIGH, 4000);
	Fdistance = (Fdistance * 0.0343) / 2;
	return (int)Fdistance;
}

void measureDistance() {
	leftDistance = getDistanceL();
	delay(40);
	rightDistance = getDistanceR();
	delay(40);
	distanceDelta = leftDistance - rightDistance;
	realDistance = (leftDistance + rightDistance) / 2;
}

void printDistance() {
	Serial.println();
	Serial.print("Left=");
	Serial.print(leftDistance);
	Serial.print("cm || Right=");
	Serial.print(rightDistance);
	Serial.print("cm || Real=");
	Serial.print(realDistance);
	Serial.print("cm || Delta=");
	Serial.print(distanceDelta);
	Serial.print("cm");
}

// Motor control methods

void forward() {
	digitalWrite(IN1, LOW);
	analogWrite(IN2, SPEEDL);
	analogWrite(IN3, (255 - SPEEDR));
	digitalWrite(IN4, HIGH);
	//	Serial.println("Forward");
}

void reverse() {
	digitalWrite(IN1, HIGH);
	analogWrite(IN2, (255 - SPEEDL));
	analogWrite(IN3, SPEEDR);
	digitalWrite(IN4, LOW);
	//	Serial.println("Reverse");
}

void left() {
	digitalWrite(IN1, HIGH);
	analogWrite(IN2, (255 - SPEEDL));
	analogWrite(IN3, (255 - SPEEDR));
	digitalWrite(IN4, HIGH);
	//	Serial.println("Left");
}

void right() {
	digitalWrite(IN1, LOW);
	analogWrite(IN2, SPEEDL);
	analogWrite(IN3, SPEEDR);
	digitalWrite(IN4, LOW);
	//	Serial.println("Right");
}

void stop() {
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, LOW);
	//	Serial.println("Stop!");
}

// Escape maneuver

void escape() {
	reverse();
	delay(300);
	stop();
	delay(50);
	left();
	delay(400);
	stop();
}

// Head movement method

void turnHeadNew(int angle_X = SERVO_X_CENTER, int angle_Y = SERVO_Y_CENTER) {
	/* Actuate the X servo */
	if (angle_X < MIN_X_ANGLE) {
		angle_X = MIN_X_ANGLE;
	} else if (angle_X > MAX_X_ANGLE) {
		angle_X = MAX_X_ANGLE;
	}
	servo_X.write(angle_X);

	/* Actuate the Y servo */
	if (angle_Y < MIN_Y_ANGLE) {
		angle_Y = MIN_Y_ANGLE;
	} else if (angle_Y > MAX_Y_ANGLE) {
		angle_Y = MAX_Y_ANGLE;
	}
	servo_Y.write(angle_Y);
}

// Power supply monitoring

double readVcc(double result = 1100.0) {
	result *= 1024.0;
	uint16_t adc_value = 0x0000;

	// Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	// Wait for Vref to settle
	delay(2);
	// Start ADC conversion
	ADCSRA |= _BV(ADSC);

	// Wait for the ADC convertsion to complete
	while (bit_is_set(ADCSRA, ADSC));

	// Get ADC reading
	adc_value = (ADCH << 8) | ADCL;

	// Back-calculate AVcc in mV
	result /= (double)adc_value;

	// This is needed to restore the DEFAULT reference
	analogRead(0);
	// This is needed to stabilize the reference before measurement;
	delay(2);

	return result;
}

void indicateVoltage(double threshold = 4200.0) {
	if (readVcc() < threshold) {
		LEDon();
	}
	else {
		LEDoff();
	}
}