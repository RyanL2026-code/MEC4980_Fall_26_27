#include <Arduino.h>
#include <PID_v1.h>

const int PIN1 = 9;  //// Set up libraries and pins 
const int PIN2 = 10;
const int PWM_FREQ = 20000;
const int PWM_BITS = 8;

// Please see sent over Videos and text file of HW

//////////////////////////////////////////// PID STUFF
/********************************************************
 * PID Basic Example
 * Reading analog input A5 to control analog PWM output 3
 ********************************************************/



///#define PIN_INPUT A5
#define PIN_OUTPUT 9

//Define Variables we'll be connecting to
double Setpoint, Input, SupplyV;

//Specify the links and initial tuning parameters
double Kp=0.8, Ki=0.2, Kd=0.05;
PID myPID(&Input, &SupplyV, &Setpoint, Kp, Ki, Kd, DIRECT);//Direct


///////////////////////////////////////////////////////////////



void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);
analogWrite(PIN_OUTPUT, 150);

     //initialize the variables we're linked to
  //Input = analogRead(PIN_INPUT);
  Setpoint = 75;

  //turn the PID on
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
}

float fullcycle;
//float DesiredRpm = 5;
//float SupplyV = 50;
float lightlevel;
float dark = 700;  // light activation level 
float RPM;

enum Lightcount {
  FirstCount,          // Enumerations for counting the rotation slit wheel, Time between two slits is 1 rotation
  SecondCount,
  Lcount
};




bool fresh = true; // Prevents double counts
Lightcount countswitch = FirstCount;

unsigned long startTime = 0; // Timer start timestamp
bool timerRunning = false; 

// Non-blocking step delay tracking variables
unsigned long lastStepTime = 0; 
const unsigned long MIN_STEP_INTERVAL = 100; // Minimum required delay between steps in milliseconds





void startTimer() {    // Creation of timer function for timing inbetween slit counts 
  startTime = millis();
  timerRunning = true;
  //Serial.println("Timer Started");
}

void resetTimer() {
  timerRunning = false;
  startTime = 0;
  //Serial.println("Timer Stopped");
}

float getElapsedTime() {
  if (!timerRunning) {
    return 0.0;
  }
  return (millis() - startTime) / 1000.0; // Spits out time passed in seconds
}

void loop() {
  analogWrite(PIN1, SupplyV);
  lightlevel = analogRead(A5);
  
  unsigned long currentMillis = millis();

  // Step 1: Detect light, start timer, wait for transition, First light hit 
  if ((lightlevel > dark) && (countswitch == FirstCount) && fresh) {
    if (currentMillis - lastStepTime >= MIN_STEP_INTERVAL) {        /// This is a form of time delay without using the DELAY() function
      startTimer();
      countswitch = (Lightcount)((countswitch + 1) % Lightcount::Lcount);
      fresh = false;
      lastStepTime = currentMillis; // Record time step occurred
      //Serial.println(" step 1");
    }
  }

  // Step 2: Detect dark transition,  wheel rotation continues 
  if ((lightlevel < dark) && (countswitch == SecondCount) && !fresh) {
    if (currentMillis - lastStepTime >= MIN_STEP_INTERVAL) {
      fresh = true;
      lastStepTime = currentMillis; // Record time step occurred
      //Serial.println(" step 2");
    }
  }

  // Step 3: Detect light transition again, calculate RPM, the slit comes back around and counts as one revolution 
  if ((lightlevel > dark) && (countswitch == SecondCount) && fresh) {
    if (currentMillis - lastStepTime >= MIN_STEP_INTERVAL) {
      //Serial.println(" step 3");
      countswitch = (Lightcount)((countswitch + 1) % Lightcount::Lcount);
      fresh = false;
      lastStepTime = currentMillis; // Record time step occurred
      
      fullcycle = getElapsedTime();
      if (fullcycle > 0) {
        RPM = 60.0 / fullcycle; // calculate actual RPM
      } else {
        RPM = 0.0;
      }
      
      //Serial.print("Elapsed Time (s): ");
      //Serial.println(fullcycle);
      //Serial.print("The RPM: ");
      Serial.println(RPM);
      Input = RPM;             // The input to the PID is RPM and the output is SupplyV
      //myPID.Compute();
      analogWrite(PIN_OUTPUT, SupplyV);
      Serial.print("This is the supplyV:");Serial.println(SupplyV);

      resetTimer();
    }
  }

// Step 2: Detect dark transition
  if ((lightlevel < dark) && (countswitch == FirstCount) && !fresh) {      // Resets back to start
    if (currentMillis - lastStepTime >= MIN_STEP_INTERVAL) {
      fresh = true;
      lastStepTime = currentMillis; // Record time step occurred
      //Serial.println(" step 4/0");
    }
  }

  myPID.Compute(); // Computes a desired valve every cycle 
  //myPID.Compute();
  //analogWrite(PIN_OUTPUT, SupplyV);

}

