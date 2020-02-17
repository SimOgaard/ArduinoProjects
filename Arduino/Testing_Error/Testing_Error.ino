#define TRIGGER 4
#define ECHO 0
#define D1 12
#define D2 13
#define Pw 14

int distance;
int Minimum = 390;
String Mellanrum;
int Speed = 390;
int SweetSpotLow = 20;
int SweetSpotHigh = 30;

void setup() {
  Serial.begin(9600);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  CheckDistance();
}

void loop(){
  
}

void UseDistance() {

    Speed = Minimum;
    analogWrite(Pw, Speed);
    Serial.println(String("State: S     ")+String("Switching: N     ")+String("Speed: ")+Speed+String(Mellanrum)+String("     Distance: ")+distance); // DEN legitt dör wtf och ibland så skippar den acc -=

  
}



