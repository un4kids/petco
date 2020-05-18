#include "Petco.h"

void setup() {
	initialization();
	LEDon();
	delay(300);
	LEDoff();
	delay(300);
}

void loop() {
	measureDistance();/* Take measurements */
	printDistance();	/* Print the measurements */
	objectFinding();	/* Determine orientation */
	turnHead();	/* Head movement */
	turnBody();	/* Body movement */
	moveBody();	/* Velocity control */

	indicateVoltage(); /* Power supply sag indication */

	delay(10);
}

void objectFinding() {
	/* Determine orientation */
	if ((distanceDelta > -2) && (distanceDelta < 2)) {
		/* Do nothing just follow */
	} else if (distanceDelta < -1) {
		if (leftDistance > 0) {
			/* Turn left */
			angle += SERVO_X_STEP;
		}
		else {
			/* Turn right */
			angle -= SERVO_X_STEP;
		}
	} else if (distanceDelta > 1) {
		if (rightDistance > 0) {
			/* Turn right */
			angle -= SERVO_X_STEP;
		}
		else {
			/* Turn left */
			angle += SERVO_X_STEP;
		}
	}
}

void turnHead() {
	/* Actuate the X servo */
	// if (angle < MIN_X_ANGLE) {
	// 	angle = MIN_X_ANGLE;
	// } else if (angle > MAX_X_ANGLE) {
	// 	angle = MAX_X_ANGLE;
	// }
	// servo_X.write(angle);

	turnHeadNew(angle);
	/* gives time for the head to turn */
	delay(20);
}

void turnBody() {
	/* Turn the body */
	if (angle < (SERVO_X_CENTER - 7) ) {
		/* turn to the right */
		right();
		delay(40);
		stop();
		angle += 5;
	} else if (angle > (SERVO_X_CENTER + 7)) {
		/* turn to the left */
		left();
		delay(40);
		stop();
		angle -= 5;
	}
}

void moveBody() {
	/* Determine speed */
	if (realDistance == 0) {
		forward();
	}
	else if (realDistance < 8) {
		/*  Add some stop with delay
		    between direction changes */
		reverse();
	}
	else if (realDistance < 16)	{
		stop();
	}
	else if (realDistance > 15)	{
		forward();
	}
	else {
		stop();
	}
}
