#include <Arduino.h>
#include <P1AM.h>
#include<math.h>
int modInput = 1;
int modOutput =2;
int modAnalogIn=3;

int pulseKey = 1;
int pinLB1 = 2;
int pinLB2 = 3;
int pinLBW = 4;
int pinLBR = 5;
int pinLBB = 6;

int linkageDistance[] = {3,7,12};
int valvePins[]={3,4,5};

enum MachineStates {
REST,
SENSE,
TRACKER,
ACTUATE,
COUNT

};

enum Colors {
WHITE,
RED,
BLUE,
COLORCOUNT
};

Colors targetColor = WHITE;



MachineStates mState = REST;


void setup(){ // the setup routine runs once:

  Serial.begin(115200);  //initialize serial communication at 115200 bits per second 
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }
}

int channelTwo;
int color = 0; 
int linkageCount = 0;
bool prevKeystate = false;
int distanceToMove = 8; 
bool currentState = false;
int defWrongColor = 10000;
int currentColor = 10000;


void TurnEverythingOff() {
 for (int i = 1; i <6 ; i++){
P1.writeDiscrete(LOW,modOutput,i);
 };
};

void ToggleConveyor(bool onOffState){
P1.writeDiscrete(onOffState,modOutput,1);
};

void ToggleCompressor(bool onOffState){
P1.writeDiscrete(onOffState,modOutput,2);
};

void UpdateRobotArmOutputs(){
  for(int i =4; i< 7; i++){
  P1.writeDiscrete(!P1.readDiscrete(modInput,i), modOutput, i +2 ) // Not is used since the light barriers are defults TRUE 
  };
};






void loop(){  // the loop routine runs over and over again forever:
  UpdateRobotArmOutputs();

  switch (mState)
  {
case MachineStates::REST:
   TurnEverythingOff();
   Serial.print("Rest State");
   //Check to se if lb 1 is broken 

   if (!P1.readDiscrete(modInput, pinLB1)){
   Serial.print("Switching State")

   // Switching state
    mState=MachineStates::SENSE;
   }
break;

case MachineStates::SENSE:
// Todo: Determine color
currentColor = min(currentColor, P1.readAnalog(modAnalogIn,1));
    Serial.print("Sense State, min color:");
    Serial.println(currentColor);
    P1.writeDiscrete(HIGH,modOutput,1);

    // wait for light barrrier 2 to be triggered 
if (!P1.readDiscrete(modInput, pinLB2)){
   Serial.print("Switching State");
   mState = MachineStates::TRACKER:
  if (currentColor < 3000) {
      targetColor = Colors::WHITE;
  } else if (currentColor < 4800){
   targetColor = Colors::RED;
  } else {
  targetColor = Colors::BLUE;

  }
}

break;

case MachineStates::TRACKER:
// Watch pulse 
ToggleCompressor(HIGH);
 bool currentState= P1.readDiscrete(modInput,pulseKey)
if(!prevKeystate && currentState){
  linkageCount++;
  // todo: determine distance to move from color and compressor 

}
prevKeystate = currentState;
if(linkageCount > linkageDistance[(int)targetColor]){
 Serial.print("Switching State");
break;
 
case MachineStates::ACTUATE:
mState = MachineStates::ACTUATE;
 ToggleConveyor(LOW);
 P1.writeDiscrete(HIGH,modOutput,valvePins[(int)targetColor]);
delay(1000)
currentState = MachineStates::Rest;
currentColor = defWrongColor;
linkageCount =0;
// todo: actuate the pistons 
// wait for a bit 
// turn everthing off?
break;

case MachineStates::COUNT:

break;
  
default:
    Serial.print("Default State");

break;
  };




	Serial.print(" Pulse ,1 ,2, 3, W,R,B, color:");
  for (int i =1; i < 7; i++){
  channelTwo = P1.readDiscrete(modInput,i);	
	Serial.println(channelTwo);	
  Serial.print(" ,  ");

  }
  color = P1.readAnalog(modAnalogIn,1);
  Serial.println(color);


  Serial.println("  ");
  for (int i = 1; i <6 ; i++){
  Serial.println("Running device ");
  Serial.println(i);  
  P1.writeDiscrete(HIGH,modOutput,i);
  delay(1000);
  P1.writeDiscrete(LOW,modOutput,i);


  }



	channelTwo = P1.readDiscrete(modInput,pulseKey);	
	Serial.println(channelTwo);			
	delay(1000);	
  
}