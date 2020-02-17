typedef enum EngineState{
  turnRight,
  turnLeft,
  turnLeft90,
  turnRight90,
  getCloserF,
  getCloserR,
};
EngineState EState;

#define triggerH 4
#define echoH 0
#define triggerV 5 //1
#define echoV 2 //4
#define D1 12
#define D2 13
#define Pw 14

bool HaveBothValues = false;
String FR;
int PotValue = 512;

Servo My_servo;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(triggerH, OUTPUT);
  pinMode(echoH, INPUT);
  pinMode(triggerV, OUTPUT);
  pinMode(echoV, INPUT);
  ForR();
}

void loop() {
  My_servo.write(degree/6.4);
}

void UseDistance(){
  if(distanceR > SweetSpotR && distanceR < SweetSpotRHigh){
    EState = turnRight;
  } else if (distanceR < SweetSpotRLow){
    EState = turnLeft;
  } else if(distanceF < SweetSpotF && DistanceR < SweetSpotR) {
    EState = turnLeft90;
  } else if(distanceF < SweetSpotF && DistanceR > SweetSpotR){
    EState = turnRight90;
  } else if(distanceF > SweetSpotF && DistanceR < SweetSpotR) {
    EState = getCloserF;
  } else if(distanceF > SweetSpotF && DistanceR > SweetSpotR){
    EState = getCloserR;
  }
}

void ForR(){
  if(FR == "F"){
    FR = "R";
    HaveBothValues = true;
  } else {
    FR = "F"; 
  }
  CheckDistance();
}

void CheckDistance(){
  long duration;
  digitalWrite(trigger+FR, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigger+FR, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigger+FR, LOW);
  duration = pulseIn(echo+FR, HIGH);
  distance+FR = (duration/2) / 29.1;
  if(HaveBothValues = false){
    ForR();
  }
  UseDistance();
}

