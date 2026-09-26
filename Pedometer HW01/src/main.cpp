#include <Arduino.h>
#include <math.h>
#include <Adafruit_BNO08x.h>
#include <AceButton.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ST7789.h>                   
#include <Adafruit_seesaw.h>
#include <time.h> 
#define BNO08X_CS 10
#define BNO08X_INT 9

//void setReports();
#define BNO08X_RESET -1

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(240, 135);
Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

enum ScreenState {
ScreenMain,
ScreenDistance,
ScreenAdjstride,
ScreenRaw,
SCRENcount
};
const char* screen[] = {"ScreenMain", "ScreenDistance", "ScreenAdjstride", "ScreenRaw"};
ScreenState SCRENmode = ScreenMain;
float stridelength = 24.0;

volatile long prevChangeTime = 0;            ////////// Setting up the buttons and interups
volatile long prevChangeTimeTwo = 0;
volatile long prevChangeTimeThree = 0;

long debounceTime = 75;
volatile bool  changeButtonFlag = false;
volatile bool  menuButtonFlag = false;
volatile bool  DecreasebuttonFlag = false;


float x = 0.0;
float y = 0.0;
float z = 0.0;
float alpha = 0.0;
float beta = 0.0;
uint16_t stepCount = 0;

void IRAM_ATTR buttonToChangeThings(){
  long now = millis();
  if (now > prevChangeTime + debounceTime){
    changeButtonFlag = true;
    prevChangeTime = now;
  }
}

void IRAM_ATTR buttonToChangeMenu(){
  long now = millis();
  if (now > prevChangeTimeTwo + debounceTime){
    menuButtonFlag = true;
    prevChangeTimeTwo = now;

  }
}

void IRAM_ATTR buttonToDecrease(){
  long now = millis();
  if (now > prevChangeTimeThree + debounceTime){
    DecreasebuttonFlag = true;
    prevChangeTimeThree = now;

  }
}

/////////////// done setting up buttons



void setReports(void) {
  if (!bno08x.enableReport(SH2_STEP_COUNTER)) {          /////// Error set up for step counter 
    Serial.println("Could not enable step counter");
  }

  
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  } else {
    Serial.println("Set accelerometer report... success!");
  }
}



void setup() {
 Serial.begin(115200);
  while (!Serial)
    delay(10);


  // Define buttons as pulldown/UP 

pinMode(0, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(0), buttonToDecrease,RISING);

pinMode(1, INPUT_PULLDOWN);
attachInterrupt(digitalPinToInterrupt(1), buttonToChangeThings,RISING);

pinMode(2,INPUT_PULLDOWN);
attachInterrupt(digitalPinToInterrupt(2), buttonToChangeMenu,RISING);


  // Set up display 
  display.init(135,240);
  display.setRotation(3);
  canvas.setTextColor(ST77XX_GREEN);
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, 1);

   Serial.println("Adafruit BNO08x test!");  /// Setup/Testing Accelerometer
  if (!bno08x.begin_I2C()) {
    Serial.println("Failed to find BNO08x chip");
    while (1) {
      delay(10);}
  Serial.println("BNO08x Found!");

  setReports(); 
}}

////////////////////////////////////// Constant Loop 


void loop (){

if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

if (bno08x.getSensorEvent(&sensorValue)) {
    // Check which report was updated before reading
    switch (sensorValue.sensorId) {
      case SH2_ACCELEROMETER: {
        float x = sensorValue.un.accelerometer.x;
        float y = sensorValue.un.accelerometer.y;
        float z = sensorValue.un.accelerometer.z;
        float alpha = atan2(x, sqrt(y*y + z*z)) * RAD_TO_DEG; 
        float beta = atan2(y, z) * RAD_TO_DEG;
        break;
      }
      case SH2_STEP_COUNTER: {
        Serial.print("Step Counter - steps: ");
        Serial.println(sensorValue.un.stepCounter.steps);
        break;
      }
    }}





//// Cycles the Menu 
if(menuButtonFlag) {
menuButtonFlag = false;
SCRENmode = (ScreenState)(((int)SCRENmode + 1) % (int)ScreenState::SCRENcount);
}
 if(changeButtonFlag){				/// Increase stride length
    if(SCRENmode == ScreenAdjstride) {
      stridelength += 1.0;}
     changeButtonFlag = false;
  }
if(DecreasebuttonFlag){			//// Decrease stride length 
    if(SCRENmode == ScreenAdjstride) {
      stridelength += -1.0;}
     DecreasebuttonFlag = false;
   
  };
  
////////////////////////////////////////////////////////////////////////////////////////
canvas.setTextColor(ST77XX_BLACK);
if (SCRENmode == ScreenMain){
  canvas.fillScreen(ST77XX_BLUE);
  canvas.setCursor(0,20);
  canvas.setTextSize(2);
  canvas.print("You are in ");
  canvas.print(screen[SCRENmode]);canvas.print(" Mode.");
  canvas.print("Step Counter - steps: ");
 canvas.print(sensorValue.un.stepCounter.steps);
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}

if (SCRENmode == ScreenDistance){
  canvas.fillScreen(ST77XX_GREEN);
  canvas.setCursor(0,20);
  canvas.setTextSize(2);
  canvas.println("You are in ");
  canvas.print(screen[SCRENmode]);  canvas.print(" Mode.");
  canvas.print("Total Distance Traveled: "); canvas.print("ft");
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}

if (SCRENmode == ScreenAdjstride){
  canvas.fillScreen(ST77XX_YELLOW);
  canvas.setCursor(0,20);
  canvas.setTextSize(2);
  canvas.println("You are in ");
  canvas.print(screen[SCRENmode]);canvas.print(" Mode.");
  canvas.print("The Current Stride Length is:  ");
  canvas.print(stridelength);
  canvas.print("inches");
  
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}

if (SCRENmode == ScreenRaw){
  canvas.fillScreen(ST77XX_ORANGE);
  canvas.setCursor(0,20);
  canvas.setTextSize(2);
  canvas.println("You are in ");
  canvas.print(screen[SCRENmode]);
  canvas.print(" Mode.");

  
  canvas.print("Accelerometer - x: ");
  canvas.print(x);
  canvas.print(" y: ");
  canvas.print(y);
  canvas.print(" z: ");
  canvas.print(z);
  canvas.print("beta: ");
  canvas.println(beta);
  canvas.print(" alpha: ");
  canvas.println(alpha);
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}

}

/*
Serial.print("You are in menu: ");
Serial.println(screen[SCRENmode]);
Serial.print(digitalRead(0));Serial.print(digitalRead(1));Serial.println(digitalRead(2));

Serial.print("The Current Stride Length is:  ");
Serial.print(stridelength);
Serial.print("inches");

}

void setReports(void) {

  
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  } else {
    Serial.println("Set accelerometer report... success!");
  }
}
*/