#define triggerL 12
#define echoL 14
#define triggerF 2
#define echoF 13

int distanceL;
int distanceF;

void setup() {
  pinMode(triggerL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(triggerF, OUTPUT);
  pinMode(echoF, INPUT);
}

void loop (){
  CheckDistance1();
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
}
