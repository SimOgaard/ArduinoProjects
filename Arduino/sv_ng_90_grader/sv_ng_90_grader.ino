#include <Servo.h>

typedef enum EngineState{
  startUp,
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

int StartSleep = 2500;

int vardeminimum = 10;
int vardemitten = 70;
int vardemaximum = 120;
int MAXSPEED = 684;
int TurnSpeed = 684;
int turnDelay = 790;

int buttonState = 0;

int Speed = TurnSpeed;

int degree;

bool testtest = false;

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
  EState = startUp;
  degree = vardemaximum;
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
}

void loop() {
  switch (EState) {
    case startUp:
      buttonState = digitalRead(buttonPin);

      if(testtest == false){
        My_servo.write(0);
      }
      
      if(buttonState == LOW){
        My_servo.write(vardemitten);
        delay(StartSleep);
        EState = turn;

        testtest = true;
        
      } else {
        EState = startUp;
      }
    break;

    case turn:

      delay(500);
      analogWrite(Pw, 580);
      delay(2000);
      
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
      EState = turn;
    break;
  }
}
