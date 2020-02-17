#include <Servo.h>

Servo my_servo;
int AI_Pot = 0;
int PotVal = 0;
int Angle = 0;
int ratio = 1024/160;

typedef enum TrafficLightStates{
  StateR,
  StateRY,
  StateG,
  StateY,
  StateYtG
};

TrafficLightStates TLState;
bool CarSense = false;

#define DO_RLed 14
#define DO_YLed 12
#define DO_GLed 13

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long diffMillis = 0;

boolean TimeLimit(int interval){
  currentMillis = millis();
  diffMillis = currentMillis - previousMillis;
  if(diffMillis > interval) {
    previousMillis = currentMillis;
    return true;
  }
  else return false;
}

void setup() {
  TLState = StateR;
  pinMode(DO_RLed, OUTPUT);
  pinMode(DO_YLed, OUTPUT);
  pinMode(DO_GLed, OUTPUT);
  Serial.begin(9600);
  my_servo.attach(13);
}

void loop() {
  PotVal = analogRead(AI_Pot);
  Angle = PotVal / ratio;
  my_servo.write(Angle);
  Serial.println(Angle);

  switch (TLState){
    case StateR:
      digitalWrite(DO_YLed, 0); //släck gul (gäller bara när den har loopat)
      digitalWrite(DO_RLed, 1); //tänd röd
      if(TimeLimit(1000)){ //om knappen trycks, kör vidare till RY
        TLState = StateRY;
      }
    break;

    case StateRY:
      digitalWrite(DO_YLed, 1);  //tänd gul
      if(TimeLimit(1000)){ //vänta 0.5s
        TLState = StateG;
      }//börja StateG
    break;

    case StateG:
      digitalWrite(DO_RLed, 0); //släck röd
      digitalWrite(DO_YLed, 0); //släck gul
      digitalWrite(DO_GLed, 1); //tänd grön
      if(TimeLimit(1000)){ //vänta 1s
        TLState = StateY;
      }
    break;

    case StateY:
      digitalWrite(DO_YLed, 1);
      digitalWrite(DO_GLed, 0);
      if(TimeLimit(1000)){
        TLState = StateR;
      }
    break;
  }
}
