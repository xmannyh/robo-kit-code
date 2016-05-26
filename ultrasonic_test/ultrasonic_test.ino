#include "NewPing.h"

// The pins for the ultrasonic sensor
  int triggerPin = 10;
  int echoPin = 11;
  int maxDistance = 200; // in cm
NewPing sonar(triggerPin, echoPin, maxDistance);

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println("cm: " + String(sonar.ping_cm()));
  delay(200);
}

