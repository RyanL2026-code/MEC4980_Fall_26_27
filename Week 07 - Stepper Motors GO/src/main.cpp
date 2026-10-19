#include <Arduino.h>
#include <AccelStepper.h>

int pinStep= 9;
int pinDir = 10;


AccelStepper onlyAxis(1, pinStep,pinDir);

void setup() {
pinMode(pinStep,OUTPUT);
pinMode(pinDir,OUTPUT);


onlyAxis.setMaxSpeed(700); // 700 steps per second
onlyAxis.setSpeed(200);

}

void loop() {

onlyAxis.runSpeed();

}


