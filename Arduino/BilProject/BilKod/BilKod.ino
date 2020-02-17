#include <Servo.h>              // inkluderar bibloteket servo
#include "EspMQTTClient.h"      // inkluderar bibloteket espmqttclitent

void onConnectionEstablished(); // anropar funktionen

EspMQTTClient client(
  "ABBIndgymIoT_2.4GHz",      // ssd
  "ValkommenHit!",            // lösen
  "192.168.0.104",            // mqtt ip
  1883,                       // mqtt port
  "jocke",                    // namn
  "apa",                      // lösen
  "microdator",               // clientens namn
  onConnectionEstablished,    // återkallar funktionen
  true,                       // tillåter uppdatering via webben
  true
);

typedef enum EngineState {                //typedef tillåtes oss att släppa taggnamnet och använda typen utan enum sökordet *. enum är en data typ
  Sensor, Drive, StartUp, Turn, FindWall  //alla cases
};
EngineState EState;                       // * kallar EngineState för EState

#define TriggerL 2                                  //definerar pins
#define EchoL 13
#define TriggerF 12
#define EchoF 14
#define D1 0
#define Pw 5
#define ButtonPin 10
unsigned long previousMillis = 0, currentMillis = 0;//definerar ints, long, bool
int DegreeMinimum = 0, DegreeMitten = 70, DegreeMax = 130, turnDelay, Degree, ServoDelay = 1000;
int StartSleep = 1500, ButtonState = 0;
int DistanceL, DistanceF = 9999, DriveTime, DriveDistanceGGR = 75, Acc = 750, lol = 1, haha = 0;
int MinstaL, MinstaF, HogstaL, HogstaF, AllDistanceL, AllDistanceF;
int SweetSpotL = 15, SweetSpotFHigh = 13, SweetSpotFLow = 13;
bool IsConnected;

Servo My_servo;                                       // kallar servot för My_servo

void setup() {                                        // körs en gång
  My_servo.attach(15);                                // definerar pin för servot
  Serial.begin(9600);                                 // sätter data hastigheten i bits pär sekund för serial monitor
  pinMode(ButtonPin, INPUT);                          // configurerar pin att antingen vara input eller output
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  EState = StartUp;                                   // går till casen startupp
}

void loop() {                                         // körs hela tiden
  client.loop();                                      // funktionen måste anropas
  switch (EState) {                                   // controllerar ordningen på cases
    case StartUp:                                     // detta är casen
      ButtonState = digitalRead(ButtonPin);           // läser av om ström går igenom pin buttonpin
      My_servo.write(DegreeMinimum);                  // ändrar värdet på 
      if (ButtonState == LOW && IsConnected == true){ // om båda är sanna
        client.publish("mess", "Knappen trycktes, startar programet"); // skickar till mess stringen
        My_servo.write(DegreeMitten);
        delay(StartSleep);                            // väntar ms
        client.publish("mess", "Försöker hitta en vägg");
        EState = FindWall;
      } else {                                        // annars
        EState = StartUp;
      }
    break;                                            // avslutar casen

    case FindWall:
      analogWrite(Pw, Acc);       // kan variera med vilken output som används
      CheckF();
      Serial.println(DistanceF);  // skriver i terminalen på en ny rad
      if(haha >= 250){
        client.publish("mess", "Hittade en vägg");
        EState = Sensor; 
      }
      if (DistanceF <= 3 && DistanceF != 0){
        haha++;
      } else {
        EState = FindWall; 
      }
    break;

    case Sensor:
      Acc = 750;
      previousMillis = millis();            // sätter undefined long till hur länge programet har körts i ms
      analogWrite(Pw, 0);
      digitalWrite(D1, HIGH);
      MinstaL = 9999;
      MinstaF = 9999;
      HogstaL = 0;
      HogstaF = 0;
      AllDistanceL = 0;
      AllDistanceF = 0;
      for(int i = 0; i < 5; i++){           // så länge i < 5 utför och lägg på på i
        CheckL();
        CheckF();

        Serial.println(DistanceL + String("     ") + DistanceF);
        
        AllDistanceL += DistanceL;
        AllDistanceF += DistanceF;
        if(DistanceL < MinstaL){ MinstaL = DistanceL; }
        if (DistanceL > HogstaL){ HogstaL = DistanceL; }
        if(DistanceF < MinstaF){ MinstaF = DistanceF; }
        if (DistanceF > HogstaF){ HogstaF = DistanceF; }
      }
      AllDistanceL -= (MinstaL + HogstaL);
      AllDistanceF -= (MinstaF + HogstaF);
      DistanceL = AllDistanceL / 3;
      DistanceF = AllDistanceF / 3;

      Serial.println(String("LEFT: ") + DistanceL + String("     FORWARD: ") + DistanceF);
      Serial.println(String("MINLEFT: ") + MinstaL + String(" MINFORWARD: ") + MinstaF + String(" HIGHLEFT: ") + HogstaL + String(" HIGHFORWARD: ") + HogstaF);
      
      if (DistanceL > SweetSpotL && haha == 0) {
        Degree = DegreeMinimum;
        turnDelay = 2050;
        client.publish("mess", String("Svänger vänster för att distancen vänster är ")+DistanceL+String("cm"));
        EState = Turn;
      } else if (DistanceF >= SweetSpotFLow && DistanceF <= SweetSpotFHigh) {  
        lol = 0;
        haha = 0;
        Degree = DegreeMax;
        turnDelay = 3050;
        client.publish("mess", String("Svänger höger för att distancen frammåt är ")+DistanceF+String("cm och distance vänster är ")+DistanceL+String("cm"));
        EState = Turn;
      } else if (DistanceF < SweetSpotFHigh) {
        lol = 1;
        DriveTime = (SweetSpotFLow -= DistanceF) * DriveDistanceGGR;
        digitalWrite(D1, LOW);
        Acc = 1023;
        client.publish("mess", String("Åker bakåt ")+(DriveTime+1000)+String("ms för att distancen frammåt är ")+DistanceF+String("cm"));
        EState = Drive;
      } else if (DistanceF > SweetSpotFHigh) {
        lol = 1;
        DriveTime = (DistanceF -= SweetSpotFHigh) * DriveDistanceGGR;
        client.publish("mess", String("Åker frammåt ")+DriveTime+String("ms för att distancen frammåt är ")+DistanceF+String("cm"));
        EState = Drive;
      }
    break;

    case Drive:
      analogWrite(Pw, Acc);
      currentMillis = millis();
      if (currentMillis - previousMillis >= DriveTime || DistanceL > SweetSpotL){ //om ena eller andra är sann
        if (Acc != 750){
          if(haha == 0) {
            My_servo.write(50);
          }
          delay(1000);
        }
        if(DistanceL > SweetSpotL){
          client.publish("mess", String("Stannar för att distance vänster är ")+DistanceF+String("cm"));
        } else {
          client.publish("mess", DriveTime+String("ms har gått"));
        }
        EState = Sensor;
      }
      if(DistanceL > 4 && Acc == 750){
        My_servo.write(50);
      } else if (DistanceL < 5 && Acc == 750){
        My_servo.write(90);
      } else{
        My_servo.write(DegreeMitten);
      }
      CheckL();
    break;

    case Turn:
      if(Degree == DegreeMinimum && lol != 0){
        My_servo.write(DegreeMitten);
        analogWrite(Pw, Acc);
        delay(650); //800
        analogWrite(Pw, 0);
      }
      My_servo.write(Degree);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      analogWrite(Pw, 0);
      My_servo.write(DegreeMitten);
      delay(ServoDelay);
      analogWrite(Pw, Acc);
      delay(300);
      analogWrite(Pw, 0);
      EState = Sensor;
    break;
  }
}

void CheckL(){                        // funktion kallad checkl
  long DurationL;
  digitalWrite(TriggerL, LOW);
  delayMicroseconds(2);               // 1000 ggr mindre än ms
  digitalWrite(TriggerL, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerL, LOW);
  DurationL = pulseIn(EchoL, HIGH);   // väntar förpin att gå från låg till hög
  DistanceL = (DurationL / 2) / 29.1;
}

void CheckF(){
  long DurationF;
  digitalWrite(TriggerF, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerF, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerF, LOW);
  DurationF = pulseIn(EchoF, HIGH);
  DistanceF = (DurationF / 2) / 29.1;
}

void onConnectionEstablished() {
  IsConnected = true;
  client.subscribe("lampa/lamp", [](const String & payload){ // subcribar till 
    client.publish("mess", "Har startat");
    EState = StartUp;
  });
}
