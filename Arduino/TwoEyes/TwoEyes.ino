#define triggerL 4
#define echoL 0
#define triggerF 2
#define echoF 5

int DistanceL;
int DistanceF;

void setup() {
  Serial.begin(9600);
  pinMode(triggerL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(triggerF, OUTPUT);
  pinMode(echoF, INPUT);
}

void loop() {
  Serial.println(DistanceL + String("L"));
  Serial.println(DistanceF + String("F"));
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
  DistanceF = (durationF/2) / 29.1;
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
  DistanceL = (durationL/2) / 29.1;
}

