#include <Servo.h>

#include "EspMQTTClient.h"

void onConnectionEstablished();

EspMQTTClient client(
 "ABBIndgymIoT_2.4GHz",           // Wifi ssid
  "ValkommenHit!",           // Wifi password
  "192.168.0.102",  // MQTT broker ip
  1883,             // MQTT broker port
  "jocke",            // MQTT username
  "apa",       // MQTT password
  "microdator",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages
);

typedef enum EngineState{
  startUp,
  turn,
  getCloserF,
  getFurtherF,
  Still,
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
int SweetSpotL = 12; //sätt detta till det testade värdet
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
}

void loop() {
  client.loop();
  switch (EState) {
    case startUp:
      if(Isconnected == true && llol == false){
        llol = true;
        client.publish("mess", "Started Up");
      }
      buttonState = digitalRead(buttonPin);
      My_servo.write(0);
      Serial.println(String(GoingToWall)+String("State: Start     ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
      if(buttonState == LOW /*&& Isconnected == true*/){
        client.publish("mess", "Button pressed starting program");
        My_servo.write(vardemitten);
        delay(StartSleep);
        CheckDistance1();
      } else {
        EState = startUp;
      }
    break;

    case turn:
      if(degree == vardeminimum){
        theway = "left";
      } else {
        theway = "right";
      }
      client.publish("mess", "Turning " + theway);
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
      client.publish("mess", "Driving forwards");
      CheckDistance1();
    break;

    case getCloserF:
      if(GoingForwards == false && Speed > Minimum) {
        degree = vardemitten;
        My_servo.write(degree);
        Speed -= AccB;
        analogWrite(Pw, Speed);
        Serial.println(String(GoingToWall)+String("State: Forward      ")+String("Switching: Yes     ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();
      } else if (Speed < MAXSPEED){
        degree = vardemitten;
        My_servo.write(degree);
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        GoingForwards = true;
        Speed += Acc;
        analogWrite(Pw, Speed);
        Serial.println(String(GoingToWall)+String("State: Forward      ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();
      } else {
        Serial.println(String(GoingToWall)+String("State: Forward      ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();
      }
    break;

    case getFurtherF:
      if (GoingForwards == true && Speed > Minimum) {
        degree = vardemitten;
        My_servo.write(degree);
        Speed -= AccB;   
        analogWrite(Pw, Speed);
        Serial.println(String(GoingToWall)+String("State: Backward     ")+String("Switching: Yes     ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();
      } else if (Speed < MAXSPEED){
        degree = vardemitten;
        My_servo.write(degree);
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        GoingForwards = false;
        Speed += Acc;
        analogWrite(Pw, Speed);
        Serial.println(String(GoingToWall)+String("State: Backward     ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();
      } else {
        Serial.println(String(GoingToWall)+String("State: Backward     ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();
      }
    break;

    case Still:
      if (Speed > Minimum){
        Speed -= AccB;
        analogWrite(Pw, Speed);
        Serial.println(String(GoingToWall)+String("State: Stop         ")+String("Switching: Yes     ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL);
        CheckDistance1();    
      } else {
        if(amountNow == amountSee){
          isInFront = true;
        }
        amountNow --;
        Speed = Minimum;
        analogWrite(Pw, Speed);
        Serial.println(String(GoingToWall)+String("State: Stop         ")+String("Switching: No      ")+String("Speed: ")+Speed+String("     DistanceF: ")+distanceF+String("     DistanceL: ")+distanceL); 
        CheckDistance1();        
      }
    break;
  }
}

void UseDistance(){ //svänger aldrig till vänster eller åker fram
  if(distanceL > SweetSpotL){
    degree = vardeminimum;
    EState = turn;
  } else if (distanceF < SweetSpotFHigh){
    degree = vardemaximum;
    EState = turn;
  } else if (distanceF < SweetSpotFLow){
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    delay(750);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    CheckDistance1();
  } else {
    CheckDistance1();
  }
}

void UseDistance2(){
  if(amountStop < amountNow){
    if(lfw != lfw2){
      client.publish("mess", lfw);
      lfw = lfw2;
    }
    if (distanceF < (SweetSpotFLow + backing)) {
      GoingToWall = "GoingToWall: Yes      ";
      EState = getFurtherF;
    } else if (distanceF > (SweetSpotFHigh + backing)){
      GoingToWall = "GoingToWall: No       ";
      EState = getCloserF;
    } else {
      if(lfw != lfww){
        lfw = lfww;
      }
      GoingToWall = "GoingToWall: Stop     ";
      EState = Still;
    }
  } else if (isInFront == false) {
    lfw = "Backing away from wall";
    lfww = "Im 15 cm from wall";
    amountNow = amountSee;
    backing = backingDistance;
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    analogWrite(Pw, TurnSpeed);
    delay(1500);
    analogWrite(Pw, Minimum);
    CheckDistance1();
  } else if (hasTurnedRight == false){
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    degree = vardemaximum;
    EState = turn;
  } else {
    //Acc = 50;
    SweetSpotFHigh = 25; //sätt detta till det testade värdet
    GoingToWall = "";
    UseDistance();
  }
}

void onConnectionEstablished() {
  client.subscribe("lampa/lamp", [](const String & payload){
    Isconnected = true;
    EState = startUp;
  });
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
  UseDistance2();  
}
