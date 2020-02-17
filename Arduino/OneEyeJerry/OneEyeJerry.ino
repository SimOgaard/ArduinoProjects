typedef enum EngineState{
  
};
EngineState EState;

#define triggerH 4
#define echoH 0
#define D1 12
#define D2 13
#define Pw 14

void setup() {
  Serial.begin(9600);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(triggerH, OUTPUT);
  pinMode(echoH, INPUT);
}

void loop() {
  
}

void UseDistance(){
  if (distance > SweetSpot){
    
  } else {
    
  }
}

void CheckDistance(){
  long duration;
  digitalWrite(triggerH, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerH, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerH, LOW);
  duration = pulseIn(echoH, HIGH);
  distance = (duration/2) / 29.1;
  UseDistance();
}
