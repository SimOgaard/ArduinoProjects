#include "EspMQTTClient.h"

void onConnectionEstablished();

EspMQTTClient client(
 "ABBIndgymIoT_2.4GHz",           // Wifi ssid
  "ValkommenHit!",           // Wifi password
  "192.168.0.115",  // MQTT broker ip
  1883,             // MQTT broker port
  "jocke",            // MQTT username
  "apa",       // MQTT password
  "microdator",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages
);

typedef enum EngineState{
  Backwards,
  Forwards,
  Still,
};
EngineState EState;

#define TRIGGER 2
#define ECHO   //5
#define D1 0
#define D2 4
#define Pw 5

int SweetSpotLow = 20;
int SweetSpotHigh = 30;
int Speed = 250;
int distance;
int Acc = 10;
int Minimum = 250;
bool GoingForwards;
String Mellanrum;

int Driven = 0;

void setup() {
  Serial.begin(9600); //  Serial.begin(115200);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  switch (EState) {
    case Backwards:
      if (GoingForwards == true && Speed > Minimum) {
        Speed -= Acc;   
        analogWrite(Pw, Speed);
        Serial.println(String("State: B     ")+String("Switching: Y     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);

        Driven += Speed;
        
        CheckDistance();
      } else if (Speed < 1030){
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        Speed += Acc;
        analogWrite(Pw, Speed);
        Serial.println(String("State: B     ")+String("Switching: N     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);

        if(GoingForwards == false){
          Driven += Speed;
        } else {

          client.publish(CASE + Driven);
          
          Driven = 0;
          Driven += Speed;
        }

        CASE = "Backwards";
        GoingForwards = false;
        CheckDistance();
      } else {
        CheckDistance();
      }
    break;
    
    case Forwards:
      if(GoingForwards == false && Speed > Minimum) {
        Speed -= Acc;
        analogWrite(Pw, Speed);
        Serial.println(String("State: F     ")+String("Switching: Y     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);
        
        myArray[Array] + Speed;
        
        CheckDistance();
      } else if (Speed < 1030){
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        Speed += Acc;
        analogWrite(Pw, Speed);
        Serial.println(String("State: F     ")+String("Switching: N     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);

        if(GoingForwards == true){
          Driven += Speed;
        } else {
          
          client.publish(CASE + Driven);
          
          Driven = 0;
          Driven += Speed;
        }

        CASE = "Forwards";
        GoingForwards == true;
        CheckDistance();
      } else {
        CheckDistance();
      }
    break;

    case Still:
      if (Speed > Minimum){
        Speed -= Acc;
        analogWrite(Pw, Speed);
        Serial.println(String("State: S     ")+String("Switching: Y     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);
        CheckDistance();    
      } else {
        Speed = Minimum;
        analogWrite(Pw, Speed);
        Serial.println(String("State: S     ")+String("Switching: N     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance); 
        CheckDistance();        
      }
    break;
  }
}

void onConnectionEstablished() {
  client.subscribe("lampa/lamp", [](const String & payload)
  CheckDistance();
}

void UseDistance(){
  client.loop();
  if(Speed < 1000) {
    Mellanrum = "  ";
  } else {
    Mellanrum = " ";
  }
  if (distance < SweetSpotLow) {
    if(CASEISTRUE == false){
      CASE = "Backwards";
    }
    CASEISTRUE = true;
    EState = Backwards;
  } else if (distance > SweetSpotHigh){
     if(CASEISTRUE == false){
      CASE = "Forwards";
    }
    CASEISTRUE = true;
    EState = Forwards;
  } else {
    EState = Still;
  }
}

void CheckDistance(){
  long duration;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  UseDistance();
}
