#include <Servo.h>
#include <VL53L0X.h>
#include <Wire.h>

typedef enum EngineState {
  StartUp,
  Turn,
  Drive,
  Distance,
};
EngineState EState;

#define D1 0
#define Pw 5
#define ButtonPin 10

const int DegreeMinimum = 0;
const int DegreeMitten = 64;
const int DegreeMax = 125;
int turnDelay = 3450;
int Degree;

int ButtonState = 0;
const int StartSleep = 1500;

const int Acc = 700;
const int SweetSpotL = 220;
int SweetSpotF = 130;
int SweetSpotFDiff = 10;
int DistanceSensorValueFram;
int DistanceSensorValueVanster;

Servo My_servo;
VL53L0X sensorFram;
VL53L0X sensorVanster;

void setup() {
  My_servo.attach(2);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  Wire.begin(12, 13);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  
  digitalWrite(14, HIGH);
  sensorFram.init(true);
  sensorFram.setTimeout(500);
  sensorFram.setAddress((uint8_t)25);

  digitalWrite(15, HIGH);
  sensorVanster.init(true);
  sensorVanster.setTimeout(500);
  sensorVanster.setAddress((uint8_t)22);
  
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  
  EState = StartUp;
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      Degree = DegreeMinimum;
      My_servo.write(Degree);
      
      if (ButtonState == LOW) {
        Degree = DegreeMitten;
        My_servo.write(Degree);
        delay(StartSleep);
        EState = Distance;
      } else {
        EState = StartUp;
      }
    break;
    
    case Distance:
      DistanceSensorValueFram = sensorFram.readRangeSingleMillimeters();
      DistanceSensorValueVanster = sensorVanster.readRangeSingleMillimeters();
      Serial.println(DistanceSensorValueFram + String("    ") + DistanceSensorValueVanster); 
      
      if (DistanceSensorValueVanster > SweetSpotL) {
        Degree = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      } else if (DistanceSensorValueFram <= (SweetSpotF+=SweetSpotFDiff) && DistanceSensorValueFram >= (SweetSpotF-=SweetSpotFDiff)) {//DU ÄR HÄR
        Degree = DegreeMax;
        turnDelay = 3450;
        EState = Turn;
      } else if (DistanceSensorValueFram < (SweetSpotF-=SweetSpotFDiff)) {
        digitalWrite(D1, LOW);
        EState = Drive;
      } else if (DistanceSensorValueFram > (SweetSpotF-=SweetSpotFDiff)) {   
        digitalWrite(D1, HIGH);
        EState = Drive;
      }
    break;
    
    case Drive:    
      analogWrite(Pw, Acc);
      EState = Distance;      
    break;
    
    case Turn:
      digitalWrite(D1, HIGH);
      My_servo.write(Degree);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      
      Degree = DegreeMitten;
      My_servo.write(Degree);
      EState = Distance;
    break;
  }
}
