#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int pin = 9;
void setup() {
  myservo.attach(pin);          // attaches the servo on pin 9 to the servo object
  pinMode(pin, OUTPUT);      // sets the digital pin as output
}

// compare all three ways!
void loop() {
  myservo.write(90);
  delay(500);
  myservo.writeMicroseconds(1500);
  delay(500);
  goToPosition(1500);
  delay(1000);
}


void goToPosition(unsigned int microseconds){
  digitalWrite(pin, HIGH);          //
  delayMicroseconds(microseconds);  // waits for a microseconds
  digitalWrite(pin, LOW);           //
}
