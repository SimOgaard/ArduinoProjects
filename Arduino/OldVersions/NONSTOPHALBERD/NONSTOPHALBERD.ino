#include <Servo.h>

typedef enum EngineState {
  StartUp,
  Drive,
};
EngineState EState;

#define D1 0
#define Pw 5
#define ButtonPin 10

int ButtonState = 0;
const int StartSleep = 1500;

int DegreeMinimum = 0;
int DegreeMitten = 64;
int DegreeMaximum = 125;

int DistanceL;
int DistanceF;
int DistanceR;
int Diff = 100;
int DistanceFLow = 100;

int Distances[];

unsigned long previousMillis = 0;
const long interval = 750;
int Sensor 0;

Servo Turn;
Servo Dist;

void setup() {
  Turn.attach(2);
  Dist.attach(14);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  pinMode(Sensor OUTPUT);
  EState = StartUp;
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      Turn.write(DegreeMinimum);
      Dist.write(0);
      if (ButtonState == LOW) {
        Turn.write(DegreeMitten);
        delay(StartSleep);
        analogWrite(Pw, 675);
        Distance();
      } else {
        EState = StartUp;
      }
    break;
    
    case Drive:
      if (DistanceF < DistanceFLow){
        digitalWrite(D1, LOW);
        Turn.write(DegreeMitten);
      } else if (DistanceL -= DistanceR > Diff){
        digitalWrite(D1, HIGH);
        Turn.write(DegreeMinimum);
      } else if (DistanceL -= DistanceR < (0-Diff)){
        digitalWrite(D1, HIGH);
        Turn.write(DegreeMaximum);
      } else {
        Turn.write(DegreeMitten);
        digitalWrite(D1, HIGH);
      }
      Distance();
    break;
  }
}

void Distance(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) {
    Dist.write(DegreeL);
    if (hej == 0){
      hej++;
      DistanceF = analogRead(Sensor); //mitten
    }
  } else if (currentMillis - previousMillis < (2*interval)) {
    Dist.write(DegreeF);
    if (hej == 1){
      hej++;
      DistanceL = analogRead(Sensor); //vänster
    }
  } else if (currentMillis - previousMillis < (3*interval)) {
    Dist.write(DegreeR);
    if (hej == 2){
      hej++;
      DistanceF = analogRead(Sensor); //mitten
    }
  } else if (currentMillis - previousMillis < (4*interval)){
    Dist.write(DegreeF);
    if (hej == 3){
      hej++;
      DistanceR = analogRead(Sensor); //höger
    }
  } else {
    hej=0;
    previousMillis = currentMillis;
    Varde();
  }
}

void dicstance(){
  unsigned long currentMillis = millis();
  for (int i = 1; i <=4; i = i + x){
    if (i==4){
      x = -1;
    } else if (i == 1){
      x = 1;
    }
    if (currentMillis - previousMillis < (i*interval){
      Dist.write(Degreeidk);
      
    }
  }
}

void dikstance(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval && interval == intervalStart) {
    Dist.write(180);
    interval = (interval*2);
  } else if (currentMillis - previousMillis >= interval {
    Dist.write(DegreeR);
    interval = intervalStart;
  }
  Distances + analogRead(Sensor);
}

