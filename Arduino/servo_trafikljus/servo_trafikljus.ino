#include <Servo.h> // includear 

typedef enum TrafficLightStates{
  StateRed, 
  StateRedYellow, 
  StateGreen, 
  StateYellow, 
};

TrafficLightStates TLState;

#define DI_CarSensor 15 
#define DO_RLed 14 
#define DO_YLed 12 
#define DO_GLed 16//13!!!!

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long diffMillis = 0;

bool buttonState = HIGH; 

boolean TimeLimit(int interval){
  currentMillis = millis();
  diffMillis = currentMillis - previousMillis;
  if(diffMillis > interval){
    previousMillis = currentMillis;
    return true;
  }
  else return false;
}

int AI_Pot = 0;
int PotValue = 0;

Servo My_servo;

void setup() { 
  My_servo.attach(13); 
  Serial.begin(9600);
  pinMode(DO_RLed, OUTPUT);
  pinMode(DO_YLed, OUTPUT);
  pinMode(DO_GLed, OUTPUT);
  pinMode(DI_CarSensor, INPUT);
  TLState = StateRed;
}

void loop() { 
  
  //buttonState = digitalRead(DI_CarSensor); 
      PotValue = analogRead(AI_Pot);
      My_servo.write(PotValue/6.4); 
      Serial.println(PotValue);
  switch (TLState){ 
    case StateRed:
      digitalWrite(DO_RLed, HIGH); 
      digitalWrite(DO_YLed, LOW);
      Serial.println("REEEEEEEEEEEEED");
    if (buttonState == HIGH){ 
        TLState = StateRedYellow; 
      } else{
        TLState = StateRed; 
      }
    break; 
  
    case StateRedYellow:
      digitalWrite(DO_YLed, HIGH);
      digitalWrite(DO_RLed, HIGH);
      Serial.println("REEEEEEEEEEEEEDYEEEEEEEEEELLOW");
      if(TimeLimit(2500)){
        TLState = StateGreen;
      }
    break;
  
    case StateGreen: 
      digitalWrite(DO_YLed, LOW); 
      digitalWrite(DO_RLed, LOW);
      digitalWrite(DO_GLed, HIGH);
      Serial.println("GREEEEEEEEEEEEEN");
      if(TimeLimit(3000)){
        TLState = StateYellow;
      }
    break;
  
    case StateYellow: 
      digitalWrite(DO_GLed, LOW);
      digitalWrite(DO_YLed, HIGH);
      Serial.println("YEEEEEEEEEEEEEEEELLOW");

        if (buttonState == HIGH) {
          TLState = StateGreen;
          digitalWrite(DO_YLed, LOW);
          } else{
            if(TimeLimit(1000))TLState = StateRed;
          }
  break;
  }
}

