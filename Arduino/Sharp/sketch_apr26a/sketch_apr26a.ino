//for laser distance sensor CJVL 53L0X v2

#include <Wire.h>
#include <VL53L0X.h>
VL53L0X sensor;
//for laser distance sensor CJVL 53L0X v2

#define Clock 5
#define Data 4

float distance;

void setup() {
  Serial.begin (9600);
  Wire.begin();
  pinMode (Data, INPUT);
  pinMode (Clock, INPUT);
//  pinMode (Servo, OUPPUT);
  sensor.init();
  sensor.setTimeout(500);
}

void loop() {
  distance = (sensor.readRangeSingleMillimeters());
  
  
  Serial.print(sensor.readRangeSingleMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
