#include <Servo.h>

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
int DegreeTurn;
int DegreeDist;

int ButtonState = 0;
const int StartSleep = 1500;

const int Acc = 700;
const int SweetSpotL = 220;
int SweetSpotF = 130;
int SweetSpotFDiff = 10;

Servo Turn;
Servo Dist;
void setup() {
  Turn.attach(2);
  Dist.attach();//123
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);  
  EState = StartUp;
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      DegreeTurn = DegreeMinimum;
      Turn.write(DegreeTurn);
      if (ButtonState == LOW) {
        DegreeTurn = DegreeMitten;
        Turn.write(DegreeTurn);
        delay(StartSleep);
        EState = Distance;
      } else {
        EState = StartUp;
      }
    break;
    
    case Distance:

      Dist.write(DegreeL)
      Dist.write(DegreeM)
      Dist.write(DegreeR)
      Dist.write(DegreeM)

      Dist.write(DegreeL)
      Dist.write(DegreeR)
      
      analogWrite(Pw, 0);
      if (DistL > SweetSpotL) {
        DegreeTurn = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      } else if (DistF <= (SweetSpotF+=SweetSpotFDiff) && DistF >= (SweetSpotF-=SweetSpotFDiff)) {
        DegreeTurn = DegreeMax;
        turnDelay = 3450;
        EState = Turn;
      } else if (DistF < (SweetSpotF-=SweetSpotFDiff)) {
        digitalWrite(D1, LOW);
        EState = Drive;
      } else if (DistF > (SweetSpotF-=SweetSpotFDiff)) {   
        digitalWrite(D1, HIGH);
        EState = Drive;
      }

      if (DistL < (DistF-DistFDiff){
        digitalWrite(D1, LOW);
        EState = Drive;
      }
      if ((DistL+DistH)>StorDist){
        //degree = stor
        if (DistL > DistH) {
          if ((DistL-=DistH) < DistVHStorSkillnad){
            //kör rakt fram
          }
          //svänger lite åt vänster
          DegreeTurn = ;
          turnDelay = ;
          EState = Turn;
        } else if (DistH > DistV){          
          if ((DistH-=DistL) < DistVHLitenSkillnad){
            //kör rakt fram
          }
          //svänga lite åt höger
          DegreeTurn = ;
          turnDelay = ;
          EState = Turn;
        }
      } else{
        //degree = liten
      }
    break;
    
    case Drive:
      analogWrite(Pw, Acc);
      delay(DriveDist);
      EState = Distance;
    break;
    
    case Turn:
      digitalWrite(D1, HIGH);
      Turn.write(DegreeTurn);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      
      DegreeTurn = DegreeMitten;
      Turn.write(DegreeTurn);
      EState = Distance;
    break;
  }
}
