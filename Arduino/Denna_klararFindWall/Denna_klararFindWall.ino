#include <Servo.h>

typedef enum EngineState {
  Sensor, Drive, StartUp, Turn, FindWall
};
EngineState EState;

#define TriggerL 2
#define EchoL 13
#define TriggerF 12
#define EchoF 14
#define D1 0
#define Pw 5
#define ButtonPin 10
//DegreeMitten = 75
unsigned long previousMillis = 0, currentMillis = 0;
int DegreeMinimum = 0, DegreeMitten = 70, DegreeMax = 130, turnDelay, Degree, ServoDelay = 1000;
int StartSleep = 1500, ButtonState = 0;
int DistanceL, DistanceF = 9999, DriveTime, DriveDistanceGGR = 75, Acc = 750, lol = 1, haha = 0;
int MinstaL, MinstaF, HogstaL, HogstaF, AllDistanceL, AllDistanceF;
int SweetSpotL = 15, SweetSpotFHigh = 13, SweetSpotFLow = 13;

Servo My_servo;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
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
        EState = FindWall;
      } else {
        EState = StartUp;
      }
    break;

    case FindWall:
      analogWrite(Pw, Acc);
      CheckF();
      Serial.println(DistanceF);
      //for loop som svänger däcken sakta vänster höger
      if(haha >= 250){
        EState = Sensor; 
      }
      if (DistanceF <= 3 && DistanceF != 0){
        haha++;
      } else {
        EState = FindWall; 
      }
    break;

    case Sensor:
      Acc = 750;
      previousMillis = millis();
      analogWrite(Pw, 0);
      digitalWrite(D1, HIGH);
      MinstaL = 9999;
      MinstaF = 9999;
      HogstaL = 0;
      HogstaF = 0;
      AllDistanceL = 0;
      AllDistanceF = 0;
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

      Serial.println(String("LEFT: ") + DistanceL + String("     FORWARD: ") + DistanceF);
      Serial.println(String("MINLEFT: ") + MinstaL + String(" MINFORWARD: ") + MinstaF + String(" HIGHLEFT: ") + HogstaL + String(" HIGHFORWARD: ") + HogstaF);
      
      if (DistanceL > SweetSpotL && haha == 0) {
        Degree = DegreeMinimum;
        turnDelay = 2050; //2250
        EState = Turn;
      } else if (DistanceF >= SweetSpotFLow && DistanceF <= SweetSpotFHigh) {  
        lol = 0; 
        haha = 0;     
        Degree = DegreeMax;
        turnDelay = 2800; //3100
        EState = Turn;
      } else if (DistanceF < SweetSpotFHigh) {
        lol = 1;
        DriveTime = (SweetSpotFLow -= DistanceF) * DriveDistanceGGR;
        digitalWrite(D1, LOW);
        Acc = 1023;
        EState = Drive;
      } else if (DistanceF > SweetSpotFHigh) {
        lol = 1;
        DriveTime = (DistanceF -= SweetSpotFHigh) * DriveDistanceGGR;
        EState = Drive;
      }
    break;

    case Drive:
      analogWrite(Pw, Acc);
      currentMillis = millis();
      if (currentMillis - previousMillis >= DriveTime || DistanceL > SweetSpotL){
        if (Acc != 750){
          if(haha == 0) {
            My_servo.write(50);
          }
          delay(1000);
        }
        EState = Sensor;
      }

      if(DistanceL > 5 && Acc == 750){
        My_servo.write(45);
      } else if (DistanceL < 4 && Acc == 750){
        My_servo.write(85);
      } else {
        My_servo.write(DegreeMitten);
      }
/*      
      if(DistanceL > 4 && Acc == 750){
        My_servo.write(49);
      } else if (DistanceL < 5 && Acc == 750){ //5
        My_servo.write(85);
      } else{
        My_servo.write(DegreeMitten);
      }
*/
      CheckL();
    break;

    case Turn:
      if(Degree == DegreeMinimum && lol != 0){
        My_servo.write(DegreeMitten);
        analogWrite(Pw, Acc);
        delay(650); //800
        analogWrite(Pw, 0);
      }
      My_servo.write(Degree);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      analogWrite(Pw, 0);
      My_servo.write(DegreeMitten);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(250);
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

