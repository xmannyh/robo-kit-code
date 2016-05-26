#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int pin = 9;
void setup() {
  myservo.attach(pin);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  myservo.writeMicroseconds(1500);              // bring the servo to approximately the mid point. range = [1000, 2000] microseconds
  delay(15);                       // waits 15ms for the servo to reach the position
}
