#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Adafruit_ST7789.h>
#include <Adafruit_seesaw.h>
#include <time.h> 

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

long getTime();
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(240, 135);
long startTime = 0;



enum hvacState {
Heating , // 0
Cooling, // 1
hCount // 2

};

enum menuState {
  TemperatureMenu, //0
  OperationMenu,    //1
  UnitMenu,    //2
  mCount        //3
};

enum tempState {
  C,      //  0
  F,      // 1 
  TCount     /// 2
};


hvacState opMode = Heating; 
menuState menuMode = TemperatureMenu; 
tempState TMode = C;
const char* hvacNames[] = {"Heating", "Cooling"};
const char* Units[]={"Celsius", "Fahrenheit"};

float targetTemp = 24.0;
volatile long prevChangeTime = 0; 
volatile long prevChangeTimeTwo = 0;
long debounceTime = 75;
volatile bool  changeButtonFlag = false;
volatile bool  menuButtonFlag = false;

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


Adafruit_BME680 bme(&Wire); // I2C

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

pinMode(1, INPUT_PULLDOWN);
attachInterrupt(digitalPinToInterrupt(1), buttonToChangeThings,RISING);

pinMode(2,INPUT_PULLDOWN);
attachInterrupt(digitalPinToInterrupt(2), buttonToChangeMenu,RISING);

  bme.setTemperatureOversampling(BME680_OS_2X);

  display.init(135,240);
  display.setRotation(3);
  canvas.setTextColor(ST77XX_GREEN);
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, 1);
startTime = millis();
 
}

void loop() {
  if (! bme.performReading()) {       // ! is some error statement 
    Serial.println("Failed to perform reading :(");
    return;
  }
  float currentTemp = bme.temperature;
  Serial.print("Temperature = ");
  Serial.print(currentTemp);
  Serial.print("  ");
  Serial.print((int)TMode);
  Serial.print(" with target ");
  Serial.print(targetTemp);
  Serial.print("Operating  in mode ");
  Serial.print((int)opMode);
  Serial.print(" in menu ");
  Serial.println(menuMode);

  /*
  canvas.fillScreen(ST77XX_MAGENTA);
  canvas.setCursor(0,20);
  canvas.print("Does this work, the temp is   ");
  canvas.print("temp: ");
  canvas.print(currentTemp);
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135);
//delay(50);
*/





if(menuButtonFlag) {
menuButtonFlag = false;
menuMode = (menuState)(((int)menuMode + 1) % (int)menuState::mCount);

}


  if(changeButtonFlag){
    if(menuMode == TemperatureMenu) {
      targetTemp += 1.0;
      if (targetTemp > 30.0) {
        targetTemp = targetTemp -10.;
      }
    }
    if (menuMode == OperationMenu){
      opMode = (hvacState)(((int)opMode +1 )% (int)hvacState::hCount);
    }
    if (menuMode == UnitMenu){
      TMode = (tempState)(((int)TMode +1 )% (int)tempState::TCount);
    }
    changeButtonFlag = false;
   
  }

  if (opMode == Heating) {
    if (currentTemp<targetTemp){
      Serial.println("Heater is on now!");
      canvas.print("Heater is on now!");}}  
  else if (opMode == Cooling){
    if (currentTemp>targetTemp){
      Serial.println("AC is on now!");
      canvas.print("AC is on now!");}}

  if (TMode == C){Serial.println("Celcius");}
  if (TMode == F){Serial.println("Fahrenheit");}
  
if (menuMode == TemperatureMenu){
canvas.fillScreen(ST77XX_BLACK);
  canvas.setCursor(0,20);
  canvas.print("Current Temperature: ");
  canvas.println(currentTemp);
  canvas.setTextSize(1.25);
  canvas.print("Target Temperature:");
  canvas.println(targetTemp);
  canvas.print("Current Operation Mode:  ");
  canvas.println(hvacNames[opMode]);
  canvas.print("Units: ");
  canvas.println(Units[TMode]);
  
  if (opMode == Heating) {
    if (currentTemp<targetTemp){
      canvas.print("Heater is on now!");}}  
  else if (opMode == Cooling){
    if (currentTemp>targetTemp){
      canvas.print("AC is on now!");}}
  
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}
if (menuMode == OperationMenu){
  canvas.fillScreen(ST77XX_BLUE);
  canvas.setCursor(0,20);
  canvas.setTextSize(2);
  canvas.println("You are in ");
  canvas.print(hvacNames[opMode]);
  canvas.print(" Mode.");
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}
if (menuMode == UnitMenu){
 canvas.fillScreen(ST77XX_RED);
  canvas.setCursor(0,20);
  canvas.print("Units  ");
  canvas.print(Units[TMode]);
  canvas.setTextSize(2);
  display.drawRGBBitmap(0,0, canvas.getBuffer(),240,135); 
}







  Serial.println();
  delay(100);
}