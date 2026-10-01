#include <math.h>
#include <Adafruit_BNO08x.h>
#include <AceButton.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>         
#include <Adafruit_ST7789.h>                   /// Reference all the needed libraries 
#include <Adafruit_seesaw.h>
using namespace ace_button;

void setReports();
#define BNO08X_RESET -1
int pinD1 = 1;
AceButton button(pinD1);

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);     
GFXcanvas16 canvas(240, 135);

enum AxisMode {
  MODE_BOTH, //0
  MODE_X,    //1
  MODE_Y,    //2       // Create Menus 
  MODE_COUNT //3
};

enum Menus {
  Menuaxis,
  MenuRaw,
  MenuCount
};

AxisMode curMode = MODE_BOTH;
Menus curMenus = Menuaxis;

// Global variables for non-blocking LED blinking
unsigned long lastRedBlink = 0;
bool redState = false;

unsigned long lastGreenBlink = 0;
bool greenState = false;

void ChangeMode(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.print(F("handleEvent(): eventType: "));
  Serial.print(AceButton::eventName(eventType));
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  if (eventType == AceButton::kEventDoubleClicked) {
    curMode = (AxisMode)((curMode + 1) % AxisMode::MODE_COUNT);       // create logic for double press and long press button functions
  }

  if (eventType == AceButton::kEventLongPressed) {
    curMenus = (Menus)((curMenus + 1) % Menus::MenuCount);
  }
}

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  pinMode(pinD1, INPUT_PULLDOWN);
  button.init(pinD1, LOW);

  // Set pin modes for LEDs once in setup
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  display.init(135,240); 
  display.setRotation(3);
  canvas.setTextColor(ST77XX_GREEN);         /// Set up the display 
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, 1);

  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(ChangeMode); 
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick); 
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);    /// Enable the button functions
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  Serial.println("Adafruit BNO08x test!");

  if (!bno08x.begin_I2C()) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");

  setReports();
}

void loop() {
  
  button.check();

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  float x = sensorValue.un.accelerometer.x;
  float y = sensorValue.un.accelerometer.y;          //// Gets accelerometer data from sensor 
  float z = sensorValue.un.accelerometer.z;

  float alpha = atan2(x, sqrt(y*y + z*z)) * RAD_TO_DEG; 
  float beta = atan2(y, z) * RAD_TO_DEG;

  unsigned long currentMillis = millis();

  ///////////////////////  RED LED (X Axis), sets screen output and led dependent on what mode/menu is active 
  if ((curMode == AxisMode::MODE_BOTH || curMode == AxisMode::MODE_X) && (curMenus == Menus::Menuaxis)) {
    int redDelay = max((int)abs(alpha * 5), 20); // Prevents 0ms delay lockup

    if (currentMillis - lastRedBlink >= redDelay) {
      lastRedBlink = currentMillis;
      redState = !redState;
      digitalWrite(5, redState ? HIGH : LOW);
    }

    canvas.fillScreen(ST77XX_BLUE);           
    canvas.setCursor(0, 20);
    canvas.setTextSize(2);
    canvas.println(" Leveling X axis");
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  } else {
    digitalWrite(5, LOW); // Ensure LED is off when mode not active
  }

  // GREEN LED (Y Axis) sets screen output and led dependent on what mode/menu is active 
  if ((curMode == AxisMode::MODE_BOTH || curMode == AxisMode::MODE_Y) && (curMenus == Menus::Menuaxis)) { 
    int greenDelay = max((int)abs(beta * 5), 20);

    if (currentMillis - lastGreenBlink >= greenDelay) {
      lastGreenBlink = currentMillis;
      greenState = !greenState;
      digitalWrite(6, greenState ? HIGH : LOW);
    }

    canvas.fillScreen(ST77XX_BLUE);           
    canvas.setCursor(0, 20);
    canvas.setTextSize(2);
    canvas.println(" Leveling Y axis");
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  } else {
    digitalWrite(6, LOW); // Ensure LED is off when mode not active
  }

  // Raw accelerometer data menu and screen output 
  if (curMenus == Menus::MenuRaw) {
    canvas.fillScreen(ST77XX_ORANGE);
    canvas.setCursor(0, 20);
    canvas.setTextSize(1);
    canvas.println(" This is the raw accelerometer data ");
    
    canvas.print(" x: "); canvas.println(x);
    canvas.print(" y: "); canvas.println(y);
    canvas.print(" z: "); canvas.println(z);
    canvas.print("beta: "); canvas.println(beta);
    canvas.print(" alpha: "); canvas.println(alpha);
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135); 
  }
}

void setReports(void) {                     //// Setting up senor Error Message 
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  } else {
    Serial.println("Set accelerometer report... success!");
  }
}