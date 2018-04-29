#include "Robot.h"

/// constructor
Robot::Robot() {
	roboSetup();
}

/// constructor
Robot::Robot(Config c) {
	roboConfig = c;
	roboSetup();
}

/// destructor/cleanup
Robot::~Robot() {
	delete sonar;
	sonar = 0;
}

/// Gets the distance away from the ping sensor (sonar/ultrasonic sensor)
/// \returns the distance in cm
int Robot::getDistance() {
	return sonar->ping_cm();
}

/// Sets the servo position to move to range: [0, 180]
/// \param pos The position to move the servo to
void Robot::setServoPos(int pos) {
	roboConfig.servoPos = pos;
	servo.write(pos);
}

/// Gets the current position of the servo
/// \returns the position of the servo
int Robot::getServoPos() {
	return roboConfig.servoPos;
}

/// Sets up the robot. Can be called to after reconfiguring the robot as well.
void Robot::roboSetup() {
	if (sonar != 0)
		sonar = new NewPing(roboConfig.triggerPin, roboConfig.echoPin, roboConfig.maxDistance);
	else {
		delete sonar;
		sonar = new NewPing(roboConfig.triggerPin, roboConfig.echoPin, roboConfig.maxDistance);
	}

	// Attach the servo object to the pin
	servo.attach(roboConfig.servoPin);

	// Set the pins to output our signals
	pinMode(roboConfig.enA, OUTPUT);
	pinMode(roboConfig.enB, OUTPUT);
	pinMode(roboConfig.motor1A, OUTPUT);
	pinMode(roboConfig.motor1B, OUTPUT);
	pinMode(roboConfig.motor2A, OUTPUT);
	pinMode(roboConfig.motor2B, OUTPUT);
}

/// moves ultrasonic sensor to check for obstacles ahead
/// \returns distance of obstacle in cm (0 if none)
int Robot::doSweep() {
	int distance = 0;
	int start = 0;
	if (roboConfig.steps < 0)
		start = 180;

	for (int servoPos = start; servoPos <= 180 && servoPos >= 0; servoPos += roboConfig.steps) {
		setServoPos(servoPos);

		// check if something is in front of the sensor
		int tempD = sonar->ping_cm();
		if (tempD > roboConfig.minDistance)
			distance = tempD;

		delay(15);
	}
	if (start == 180)
		setServoPos(0);
	else if (start == 0)
		setServoPos(180);

	// invert the direction of the steps
	roboConfig.steps *= -1;

	// report distance

	return distance; // no obstacles
}

/// Sends an analog signal to the motors
/// \param leftSpeed The value to set the motor to. range: [-255,255]
/// \param rightSpeed The value to set the motors to. range: [-255,255]
void Robot::drive(int leftSpeed, int rightSpeed) {
	// Make sure the values are in range.
	checkRange(&leftSpeed);
	checkRange(&rightSpeed);

	analogWrite(roboConfig.enA, abs(leftSpeed));
	analogWrite(roboConfig.enB, abs(rightSpeed));

	setMotorStates(leftSpeed, rightSpeed);
}

/// Sets the motor states to go forward, backward, or stop
/// \param leftVal The left value you're sending to the motors
/// \param rightVal The right value you're sending to the motors
void Robot::setMotorStates(int leftVal, int rightVal) {
	// create an array of values
	// reverse one value so that both are going in the same direction
	int vals[2] = { -leftVal, rightVal };
	int motors[4] = { roboConfig.motor1A, roboConfig.motor1B, roboConfig.motor2A, roboConfig.motor2B };

	for (int i = 0; i < 2; i++) {
		// Set the motors to stop
		if (vals[i] == 0) {
			digitalWrite(motors[2 * i], LOW);
			digitalWrite(motors[(2 * i) + 1], LOW);
		}
		// Set the motors to go backwards
		else if (vals[i] < 0) {
			digitalWrite(motors[2 * i], HIGH);
			digitalWrite(motors[(2 * i) + 1], LOW);
		}
		// Set motors to go forward
		else if (vals[i] > 0) {
			digitalWrite(motors[2 * i], LOW);
			digitalWrite(motors[(2 * i) + 1], HIGH);
		}
	}
}

/// Makes sure the input is within the required range
/// \param val The input to check
void Robot::checkRange(int *val) {
	if (*val < -255)
		*val = -255;
	else if (*val > 255)
		*val = 255;
}
