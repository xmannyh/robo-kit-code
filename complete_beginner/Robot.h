#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "NewPing.h"
#include <Servo.h>

struct Config {
	// the pin for the servo
	int servoPin = 9;
	int steps = 3;
	int servoPos = 0;

	// The pins for the ultrasonic sensor
	int triggerPin = 10;
	int echoPin = 11;

	// The pins for the DC Motors
	// Pins controlling the speed of the motor via PWM
	int enA = 5;
	int enB = 6;
	// Pins controlling the direction the motors are going
	int motor1A = 7;
	int motor1B = 2;
	int motor2A = 3;
	int motor2B = 4;

	// Ultrasonic sensor limits (cm)
	int maxDistance = 200;
	int minDistance = 2;
};


class Robot {
public:
	Robot();
	Robot(Config c);
	~Robot();

	int getDistance();

	void setServoPos(int pos);
	int getServoPos();

	void roboSetup();
	int doSweep();
	void drive(int leftSpeed, int rightSpeed);

	Config roboConfig;

private:
	void checkRange(int *val);
	void setMotorStates(int leftVal, int rightVal);

	// objects handling control over servos and our sensor.
	Servo servo;
	NewPing *sonar;
};

#endif //__ROBOT_H__
