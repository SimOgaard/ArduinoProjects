#include <Servo.h>

typedef enum EngineState {
  StartUp,
  SendInfo,
};
EngineState EState;

#define D1 0
#define Pw 5
#define ButtonPin 10

int ButtonState = 0;
const int StartSleep = 1500;

int distdegree = 0;
int Distances[65];
int Sensor = 0;
int delaysensor = 30;

int i = 0;
int x;
int anvandbartvarde = 0;
int allaforedrar = 0;
int foredraramount = 0;
int mittenvarde = 0;
int anvandbartindex = 0;
int medelmitten = 0;
int foredrar = 0;
int foredrarindex = 0;

int backingdist = 500;
int foredrardist = 175;

Servo Turn;
Servo Dist;

void setup() {
  Turn.attach(2);
  Dist.attach(14);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  EState = StartUp;
}

void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      Turn.write(0);
      Dist.write(distdegree);
      if (ButtonState == LOW) {
        Turn.write(64);
        delay(StartSleep);
        Distance();
      } else {
        EState = StartUp;
      }
    break;
    
    case SendInfo:
      //analogWrite(Pw, 675);
      Serial.println(String("  (") + anvandbartindex + String(" ") + medelmitten + String(" ") + foredrarindex + String(")"));
      //skicka de tre varden ovan och Distances[] arrayen
      Distance();
    break;
  }
}

void Distance(){
  if (distdegree == 0){
    x = 1;
    i++;
  } else {
    x = -1;
    i--;
  }
  anvandbartvarde = 0;
  allaforedrar = 0;
  foredraramount = 0;
  mittenvarde = 0;
  while (i < 62 && i > 0){ //få servot att snurra saktare??????? TAR INTE VÄRDET PÅ INDEX 0!!!!!!!!1
    Distances[i] = analogRead(Sensor);
    i += x;
    Dist.write(distdegree);
    distdegree += x*3;
    
    Serial.print(Distances[i] + String("(") + i + String(")  "));

    if (Distances[i] > foredrardist && i != 0 && i != 62){
      if(Distances[i] < anvandbartvarde){
        anvandbartvarde = Distances[i];
        anvandbartindex = (i*2);
      }
    } else if (i != 0 && i != 62){
      allaforedrar+=i;
      foredraramount++;
    }
    if (i >= 28 && i <= 34){
      mittenvarde += Distances[i];
    }
    delay(delaysensor);
  }

  medelmitten = mittenvarde/6;
  if (medelmitten > backingdist){
    digitalWrite(D1, LOW);
    Turn.write(64);
    EState = SendInfo;
  }
  digitalWrite(D1, HIGH);
  if(foredraramount<=3){
    Turn.write(anvandbartindex);
    EState = SendInfo;
  } else {
    foredrar = allaforedrar/foredraramount;
    foredrarindex = foredrar * 2;
    Turn.write(foredrarindex);
    EState = SendInfo;
  }
}

