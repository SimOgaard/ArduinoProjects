#include <Servo.h>

typedef enum EngineState {
  StartUp,
  Drive,
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
int delaysensor = 20;

int i = 0;
int x;

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
    
    case Drive:
//      analogWrite(Pw, 675);
//      Turn.write(i); //indexet i arrayen som den valde att åka till
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
  while (i < 62 && i > 0){
    i += x;
    Distances[i] = analogRead(Sensor);
    Dist.write(distdegree);
    distdegree += x*3;
    Serial.println(Distances[i] + String("        ") +i + String("        ") + distdegree);
    delay(delaysensor);
  }

  //använd arrayen
  //kolla om värdert är större än nära och lägg indexen på det värdet som kavlificeras i en ny array
  //sedan kolla om antalet värden i den nya arrayen är större än 5 om de inte är det backa/gör en 180 grader sväng
  //finns det mer än 5 värden kolla efter värdenas index som är nära varandra
  //splitta arrayen till flera arrays som innehåller de värden som är nära varandra 
  //ta ett medelvärde på den med störst antal värden
  EState = Drive;
}

