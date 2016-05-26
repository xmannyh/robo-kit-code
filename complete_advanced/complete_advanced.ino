#include "NewPing.h"
#include <Servo.h>

int doSweep();
void drive(int leftSpeed, int rightSpeed);
void checkRange(int *val);
void setMotorStates(int leftVal, int rightVal);

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

// objects handling control over servos and our sensor.
Servo servo;
NewPing sonar(triggerPin, echoPin, maxDistance);

void setup() {
  // Attach the servo object to the pin
  servo.attach(servoPin);

  // Output debug information/distances
  Serial.begin(9600);

  // Set the pins to output our signals
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
}

void loop() {
  // check if there are no obstacles (greater than 50 cm)
  if (doSweep() > 50) {
    // move the robot
    drive(120, 120);
  }

  drive(120, 120);
  delay(1000);
  // Stop the robot
  drive(0, 0);
  delay(1000);
}

/// moves ultrasonic sensor to check for obstacles ahead
/// \returns distance of obstacle in cm (0 if none)
int doSweep() {
  int distance = 0;
  int start = 0;
  if (steps < 0)
    start = 180;

  for (int servoPos = start; servoPos <= 180 && servoPos >= 0; servoPos += steps) {
    servo.write(servoPos);

    // check if something is in front of the sensor
    int tempD = sonar.ping_cm();
    if (tempD > minDistance)
      distance = tempD;

    delay(15);
  }

  //
  steps *= -1;
  Serial.println(steps);

  // report distance
  Serial.println("cm: " + String(distance));

  return distance; // no obstacles
}

/// Sends an analog signal to the motors
/// \param leftSpeed The value to set the motor to. range: [-255,255]
/// \param rightSpeed The value to set the motors to. range: [-255,255]
void drive(int leftSpeed, int rightSpeed) {
  // Make sure the values are in range.
  checkRange(&leftSpeed);
  checkRange(&rightSpeed);

  analogWrite(enA, abs(leftSpeed));
  analogWrite(enB, abs(rightSpeed));

  setMotorStates(leftSpeed, rightSpeed);
}

/// Sets the motor states to go forward, backward, or stop
/// \param leftVal The left value you're sending to the motors
/// \param rightVal The right value you're sending to the motors
void setMotorStates(int leftVal, int rightVal) {
  // create an array of values
  // reverse one value so that both are going in the same direction
  int vals[2] = { -leftVal, rightVal };
  int motors[4] = { motor1A, motor1B, motor2A, motor2B };
  
  Serial.println(String(leftVal) + "," + String(rightVal));
  for (int i = 0; i < 2; i++) {
    // Set the motors to stop
    if (vals[i] == 0) {
      digitalWrite(motors[2 * i], LOW);
      digitalWrite(motors[(2 * i) + 1], LOW);
      Serial.println("stopped");
    }
    // Set the motors to go backwards
    else if (vals[i] < 0) {
      digitalWrite(motors[2 * i], HIGH);
      digitalWrite(motors[(2 * i) + 1], LOW);
      Serial.println("reversed");
    }
    // Set motors to go forward
    else if (vals[i] > 0) {
      digitalWrite(motors[2 * i], LOW);
      digitalWrite(motors[(2 * i) + 1], HIGH);
      Serial.println("forward");
    }
  }
}

/// Makes sure the input is within the required range
/// \param val The input to check
void checkRange(int *val) {
  if (*val < -255)
    *val = -255;
  else if (*val > 255)
    *val = 255;
}
