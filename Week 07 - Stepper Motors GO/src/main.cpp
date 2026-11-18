#include <Arduino.h>
#include <AccelStepper.h>

int pinStep= 9;
int pinDir = 10;


AccelStepper onlyAxis(1, pinStep,pinDir);

void setup() {
pinMode(pinStep,OUTPUT);
pinMode(pinDir,OUTPUT);


onlyAxis.setMaxSpeed(10000); // 700 steps per second
onlyAxis.setSpeed(4000);
onlyAxis.setAcceleration(3000);

}

void loop() {

onlyAxis.runSpeed();
delay(1000);
onlyAxis.stop();
delay(500);

}


