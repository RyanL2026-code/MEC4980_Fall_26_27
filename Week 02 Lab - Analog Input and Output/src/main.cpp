#include <Arduino.h>
#include <string.h>
#include <Ticker.h>

// put function declarations here:
int myFunction(int, int);
volatile String printSentence = "";
long prevSampleTime = 0;
long timeBetweenSampleMs = 100;
void myfun
Ticker newTimerFn (myFunction, timeBetweenSampleMs, 0, MILLIS);

void setup() {
   Serial.begin(9600);  // The serial was launching too slow so we put it first and fixed the issue
   delay(1000);
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(0,INPUT_PULLUP);
  pinMode(1, INPUT_PULLDOWN);
  pinMode(2, INPUT_PULLDOWN);

  newTimerFn.start(); 
 
}


void loop() {
  
  // put your main code here, to run repeatedly:
  long currentTime = millis();
  newTimerFun.update();
  if (strcmp(printSentence, ""))
Serial.print


  if (millis() > prevSampleTime + timeBetweenSampleMs) {
   
  prevSampleTime = currentTime;
}
}

// put function definitions here:
void myFunction(int x, int y) {
   printSentence = "D0,D1,D2, A0: ";
    printSentence += (digitalRead(0) + ","+ String(digitalRead(1)) + " , " + digitalRead(2));
    printSentence += " , " + String(analogRead(A0));
  return x + y;
}