#include <Wire.h>
#include <VL53L0X.h>
VL53L0X sensor;

#define Clock 5
#define Data 4

float distance;

void setup() {
  Serial.begin (9600);
  Wire.begin();
  pinMode (Data, INPUT);
  pinMode (Clock, INPUT);
  sensor.init();
  sensor.setTimeout(500);
}

void loop() {
  Serial.println(sensor.readRangeSingleMillimeters());

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
}
