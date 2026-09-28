#include <Arduino.h>

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