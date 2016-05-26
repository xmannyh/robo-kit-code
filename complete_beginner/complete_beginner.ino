#include "Robot.h"

// Declare our robot object to control our robot
Robot myRobot;

// runs only once
void setup() {
  //Serial.begin(9600);
}

// runs over and over
void loop() {
  int dist = myRobot.doSweep();
  // check if there are no obstacles (greater than 50 cm)
  if (dist > 50) {
    // move the robot
    myRobot.drive(-120, -120);
  }
  else {
    myRobot.drive(120, 120);
  }
  delay(500);
  myRobot.drive(0, 0);
  delay(500);
}
