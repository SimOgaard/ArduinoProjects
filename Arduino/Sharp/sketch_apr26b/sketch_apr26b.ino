#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  sensor.setSignalRateLimit(0.25); // 0.25 MCPS
  
#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 14);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 10);
#endif
#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(200000);
#endif
}

void loop()
{
  Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
  delay(500);
}

