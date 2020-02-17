#include <Servo.h>

typedef enum EngineState {
  Sensor, Drive, StartUp, FindWall, Turn
};
EngineState EState;

#define TriggerL 12
#define EchoL 14
#define TriggerF 2
#define EchoF 13
#define D1 0
#define Pw 5
#define ButtonPin 10

unsigned long previousMillis = 0, currentMillis = 0;
int DegreeMinimum = 0, DegreeMitten = 65, DegreeMax = 125, turnDelay, Degree, ServoDelay = 1000;
int StartSleep = 1500, ButtonState = 0;
int DistanceL, DistanceF, DriveTime, DriveDistanceGGR = 200, Acc = 650;
int SweetSpotL = 15, SweetSpotFHigh = 13, SweetSpotFLow = 12;

Servo My_servo;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      My_servo.write(DegreeMinimum);
      if (ButtonState == LOW) {
        My_servo.write(DegreeMitten);
        delay(StartSleep);
        EState = FindWall;
      } else {
        EState = StartUp;
      }
    break;

    case FindWall:
      CheckF();
      digitalWrite(D1, HIGH);
      if (DistanceF < SweetSpotFLow){
        delay()
      }
    break;

    case Sensor:
      previousMillis = millis();
      analogWrite(Pw, 0);
      digitalWrite(D1, HIGH);
      CheckL();
      CheckF();
      if (DistanceL > SweetSpotL && ButtonState == 3) {
        Degree = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      } else if (DistanceF >= SweetSpotFLow && DistanceF <= SweetSpotFHigh) {        
        Degree = DegreeMax;
        turnDelay = 3450;
        ButtonState = 3;
        EState = Turn;
      } else if (DistanceF < SweetSpotFLow) {
        DriveTime = (SweetSpotFLow -= DistanceF) * DriveDistanceGGR;
        digitalWrite(D1, LOW);
        EState = Drive;
      } else if (DistanceF > SweetSpotFHigh) {
        DriveTime = (DistanceF -= SweetSpotFHigh) * DriveDistanceGGR;
        EState = Drive;
      }
    break;

    case Drive:
      currentMillis = millis();
      if (currentMillis - previousMillis >= DriveTime || DistanceL > SweetSpotL){
        EState = Sensor;
      }
      CheckL();
    break;

    case Turn:
      My_servo.write(Degree);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      analogWrite(Pw, 0);
      My_servo.write(DegreeMitten);
      delay(ServoDelay);
      EState = Sensor;
    break;
  }
}

void CheckL(){
  long DurationL;
  digitalWrite(TriggerL, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerL, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerL, LOW);
  DurationL = pulseIn(EchoL, HIGH);
  DistanceL = (DurationL / 2) / 29.1;
}

void CheckF(){
  long DurationF;
  digitalWrite(TriggerF, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerF, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerF, LOW);
  DurationF = pulseIn(EchoF, HIGH);
  DistanceF = (DurationF / 2) / 29.1;
}

