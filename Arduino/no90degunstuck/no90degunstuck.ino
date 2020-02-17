#include <Servo.h>

typedef enum EngineState {
  Sensor, Drive, StartUp, Turn
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
int DegreeMinimum = 10, DegreeMitten = 75, DegreeMax = 140, turnDelay, Degree, ServoDelay = 1000;
int StartSleep = 1500, ButtonState = 0;
int DistanceL, DistanceF, DriveTime, DriveDistanceGGR = 75, Acc = 650;
int MinstaL, MinstaF, HogstaL, HogstaF, AllDistanceL, AllDistanceF;
int SweetSpotL = 15, SweetSpotFHigh = 16, SweetSpotFLow = 15;

Servo My_servo;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  EState = StartUp;
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      My_servo.write(DegreeMinimum);
      if (ButtonState == LOW){
        My_servo.write(DegreeMitten);
        delay(StartSleep);
        EState = Sensor;
      } else {
        EState = StartUp;
      }
    break;

    case Sensor:
      previousMillis = millis();
      analogWrite(Pw, 0);
      digitalWrite(D1, HIGH);
      MinstaL, MinstaF = 9999;
      HogstaL, HogstaF, AllDistanceL, AllDistanceF = 0;
      for(int i = 0; i < 5; i++){
        CheckL();
        CheckF();

        Serial.println(DistanceL + String("     ") + DistanceF);
        
        AllDistanceL += DistanceL;
        AllDistanceF += DistanceF;
        if(DistanceL < MinstaL){ MinstaL = DistanceL; }
        if (DistanceL > HogstaL){ HogstaL = DistanceL; }
        if(DistanceF < MinstaF){ MinstaF = DistanceF; }
        if (DistanceF > HogstaF){ HogstaF = DistanceF; }
      }
      AllDistanceL -= (MinstaL + HogstaL);
      AllDistanceF -= (MinstaF + HogstaF);
      DistanceL = AllDistanceL / 3;
      DistanceF = AllDistanceF / 3;
      DistanceFF = AllDistanceF / 3;

      Serial.println(String("LEFT: ") + DistanceL + String("     FORWARD: ") + DistanceF);
      Serial.println(String("MINLEFT: ") + MinstaL + String(" MINFORWARD: ") + MinstaF + String(" HIGHLEFT: ") + HogstaL + String(" HIGHFORWARD: ") + HogstaF);
      
      if (DistanceL > SweetSpotL) {
        Degree = DegreeMinimum;
        turnDelay = 2250;
        EState = Turn;
      } else if (DistanceF >= SweetSpotFLow && DistanceF <= SweetSpotFHigh) {        
        Degree = DegreeMax;
        turnDelay = 2600;
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
      analogWrite(Pw, Acc);
      currentMillis = millis();
      if (currentMillis - previousMillis >= DriveTime || DistanceL > SweetSpotL){
        CheckF();
        if (DistanceF < (DistanceFF+10) && DistanceF > DistanceFF-10){
          //backa sedan sväng åt vänster
        }
        EState = Sensor;
      }
      if(DistanceL > 7){
        My_servo.write(60);
      } else if (DistanceL < 6){
        My_servo.write(90);
      } else{
        My_servo.write(DegreeMitten);
      }
      CheckL();
    break;

    case Turn:
      if(Degree == DegreeMinimum){
        analogWrite(Pw, Acc);
        delay(750);
        analogWrite(Pw, Acc);
      }
      My_servo.write(Degree);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      analogWrite(Pw, 0);
      My_servo.write(DegreeMitten);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(500);
      analogWrite(Pw, 0);
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

