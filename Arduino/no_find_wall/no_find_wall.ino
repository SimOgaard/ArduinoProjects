#include <Servo.h>

typedef enum EngineState{
  turn,
};
EngineState EState;

#define triggerL 12
#define echoL 14
#define triggerF 2
#define echoF 13
#define D1 0
#define D2 4
#define Pw 5
#define buttonPin 10


int vardeminimum = 10;
int vardemitten = 75; //70
int vardemaximum = 120;
int TurnSpeed = 684;
int turnDelay = 790;
int StartSleep = 2500;
int distanceL;
int distanceF;
int SweetSpotL = 13; //sätt detta till det testade värdet
int SweetSpotFHigh = 10; //sätt detta till längden från sensorn till däcken +2
int SweetSpotFLow = 2; //sätt detta till längden från sensorn till däcken -2
int buttonState = 0;
int MAXSPEED = 500;
int Minimum = 380;
int backing;
bool isInFront = false;
bool hasTurnedRight = false;
int backingDistance = 10;// distancen den behöver för att backa
int Speed = Minimum;
int degree;
bool GoingForwards;
int Acc = 10;
int amountStop = 0;
int amountNow = 3; // får inte vara lika med måste vara mindre
int amountSee = 4; //denhär
String GoingToWall;
int AccB = 50;
bool Isconnected = false;
int driveSpeed = 425;
String lfw2;
String lfw = "Looking for wall";
String lfww = "Found wall";
String theway;
bool llol = false;

Servo My_servo;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(triggerL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(triggerF, OUTPUT);
  pinMode(echoF, INPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
}

void loop() {
  switch (EState) {
    case turn:
      Serial.println(String(GoingToWall)+String("State: Turn         ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);      
      analogWrite(Pw, 0);
      delay(400);
      My_servo.write(degree);
      delay(800);
      Speed = TurnSpeed;
      analogWrite(Pw, Speed);
      delay(turnDelay);
      analogWrite(Pw, 0);
      My_servo.write(vardemitten);
      delay(800);
      hasTurnedRight = true;
      Speed = driveSpeed;
      analogWrite(Pw, Speed);
      CheckDistance1();
    break;
  }
}

void UseDistance(){
  if(distanceL > SweetSpotL){
    degree = vardeminimum;
    EState = turn;
  } else if (distanceF < SweetSpotFLow){
    degree = vardemaximum;
    EState = turn;
  } else {
    CheckDistance1();
  }
}

void CheckDistance1(){
  long durationF;
  digitalWrite(triggerF, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerF, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerF, LOW);
  durationF = pulseIn(echoF, HIGH);
  distanceF = (durationF/2) / 29.1;
  CheckDistance2();
}

void CheckDistance2(){
  long durationL;
  digitalWrite(triggerL, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerL, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerL, LOW);
  durationL = pulseIn(echoL, HIGH);
  distanceL = (durationL/2) / 29.1;    
  UseDistance();  
}
