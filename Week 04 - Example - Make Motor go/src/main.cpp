#include <Arduino.h>
/*
const int PIN1 = 9;
const int PIN2 = 10;
const int PWM_FREQ = 20000;
const int PWM_BITS = 8;

void setup() {
  ledcAttach(PIN1, PWM_FREQ, PWM_BITS);
  ledcAttach(PIN2, PWM_FREQ, PWM_BITS);
}

void loop() {
  ledcWrite(PIN1, 128);
  ledcWrite(PIN2, 0);
}





































/*#include <Arduino.h>

int ch1 = 0;
int ch2 = 1;

void setup (){
ledcAttachPin(9,0);
ledcAttachPin(10,1);
}

void loop () {

ledcWrite(ch1,255);
delay(1000);
ledcWrite(ch1,170);
delay(1000);
ledcWrite(ch1,0);
delay(1000);
ledcWrite(ch2,170);
delay(1000);
ledcWrite(ch2,250);
delay(1000);
ledcWrite(ch2,0);











/*  analogWrite(9,255);
delay(1000);
analogWrite(9,127);
delay(100);
analogWrite(9,0);
delay(1000);
analogWrite(10,255);
delay(1000);
analogWrite(10,0);
*/

}