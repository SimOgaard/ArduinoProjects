typedef enum EngineState{
  Backwards,
  Forwards,
  Still,
};
EngineState EState;

#define TRIGGER 4
#define ECHO 0
#define D1 12
#define D2 13
#define Pw 14

int SweetSpotLow = 20;
int SweetSpotHigh = 30;
int Speed = 250;
int distance;
int Acc = 10;
int Minimum = 250;
bool GoingForwards;
String Mellanrum;

void setup() {
  Serial.begin(9600);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  CheckDistance();
}

void loop() {
  switch (EState) {
    case Backwards:
      if (GoingForwards == true && Speed > Minimum) {
        Speed -= Acc;   
        analogWrite(Pw, Speed);
        Serial.println(String("State: B     ")+String("Switching: Y     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);
        CheckDistance();
      } else if (Speed < 1030){
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        GoingForwards = false;
        Speed += Acc;
        analogWrite(Pw, Speed);
        Serial.println(String("State: B     ")+String("Switching: N     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);
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
        CheckDistance();
      } else if (Speed < 1030){
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        GoingForwards = true;
        Speed += Acc;
        analogWrite(Pw, Speed);
        Serial.println(String("State: F     ")+String("Switching: N     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance);
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

void UseDistance(){
  if(Speed < 1000) {
    Mellanrum = "  ";
  } else {
    Mellanrum = " ";
  }
  if (distance < SweetSpotLow) {
    EState = Backwards;
  } else if (distance > SweetSpotHigh){
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
