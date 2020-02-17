#include <Servo.h>

typedef enum EngineState {
  StartUp,
  Drive,
};
EngineState EState;

#define D1 0
#define Pw 5
#define ButtonPin 10

int Sensor = 0;
int ButtonState = 0;
const int StartSleep = 1500;

int TurnDegree;
int DistanceL;
int DistanceF;
int DistanceR;
int Backingdist = 550;

Servo Turn;
Servo Dist;

void setup() {
  Turn.attach(2);
  Dist.attach(14);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  EState = StartUp;
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      Turn.write(0);
      Dist.write(0);
      if (ButtonState == LOW) {
        Turn.write(64);
        delay(StartSleep);
        Distance();
      } else {
        EState = StartUp;
      }
    break;
    
    case Drive:
      Turn.write(TurnDegree);
      analogWrite(Pw, 675);
      delay(1000);
      Distance();
    break;
  }
}

void Distance(){
  analogWrite(Pw, 0);
  DistanceL = analogRead(Sensor);
  Dist.write(90);
  delay(500);
  DistanceF = analogRead(Sensor);
  Dist.write(180);
  delay(500);
  DistanceR = analogRead(Sensor);
  Dist.write(0);

  if(DistanceF > Backingdist){
    TurnDegree = 64;
    EState = Drive;
  } else {
    if(DistanceL < DistanceR){
      TurnDegree = 0;
      EState = Drive;
    } else {
      TurnDegree = 124;
      EState = Drive;
    }
  }
}

