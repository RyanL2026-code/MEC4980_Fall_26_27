#include <Arduino.h>

#include <Adafruit_BNO08x.h>

// For SPI mode, we need a CS pin
#define BNO08X_CS 10
#define BNO08X_INT 9

#include <Joystick_ESP32S2.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  1, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering


// For SPI mode, we also need a RESET
//#define BNO08X_RESET 5
// but not for I2C or UART
#define BNO08X_RESET -1

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void setReports();

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

    // Initialize Joystick Library
  Joystick.begin(1,INPUT_PULLDOWN);
  Joystick.setXAxisRange(-10, 10);
  Joystick.setYAxisRange(-10, 10);
  Joystick.setZAxisRange(-10, 10);

}


void setReports(void) {
Serial.println("Setting desired reports");
if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
}};

void loop(){
 Joystick.setButton(1, currentButtonState);
 Joystick.setXAxis((int)sensorValue.un.accelerometer.x);
 Joystick.setYAxis((int)sensorValue.un.accelerometer.y);
 Joystick.setZAxis((int)sensorValue.un.accelerometer.z);


 Serial.print("Accelerometer - x: ");
    Serial.print(sensorValue.un.accelerometer.x);
    Serial.print(" y: ");
    Serial.print(sensorValue.un.accelerometer.y);
    Serial.print(" z: ");
    Serial.println(sensorValue.un.accelerometer.z);
}