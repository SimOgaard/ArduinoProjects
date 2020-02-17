#include <Servo.h>
//#include "EspMQTTClient.h"
/*
  void onConnectionEstablished();

  EspMQTT//client //client(
  "ABBIndgymIoT_2.4GHz",
  "ValkommenHit!",
  "192.168.0.102",
  1883,
  "jocke",
  "apa",
  "microdator",
  onConnectionEstablished,
  true,
  true
  );
*/
typedef enum EngineState {
  StartUp, Turn, GetCloserF, GetFurtherF, Still, CheckAndUseDistance,
};
EngineState EState;

#define TriggerL 12
#define EchoL 14
#define TriggerF 2
#define EchoF 13
#define D1 0
#define D2 4
#define Pw 5
#define ButtonPin 10

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

String Direction; //den kan inte backa?
bool IsConnected = false;
int RunTimeAmount = 4, DegreeMinimum = 0, DegreeMitten = 65, DegreeMax = 125, turnDelay = 3450, StartSleep = 1500, DistanceL, DistanceF, ButtonState = 0, Degree, StartSteg = 4, CheckAmount = 100, AllDistancesF, AllDistancesL, DriveDistance, DriveTime, CheckAmount2 = 10, oldDistanceL, DistanceL2;
int MinimumSpeed = 0, Acc = 700, DriveDistanceGGR = 125;
int SweetSpotL = 15, SweetSpotFHigh = 13, SweetSpotFLow = 12, AdjustingForTurnDistance = 10;

Servo My_servo;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT), pinMode(D2, OUTPUT);
}

void loop() {
  //client.loop();
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      Degree = DegreeMinimum;
      My_servo.write(Degree);
      if (ButtonState == LOW /*&& IsConnected == true*/) {
        //client.publish("mess", "Knappen trycktes, startar programet");
        Degree = DegreeMitten;
        My_servo.write(Degree);
        delay(StartSleep);
        //client.publish("mess", "Försöker hitta en vägg");
        EState = CheckAndUseDistance;
      } else {
        EState = StartUp;
      }
    break;

    case CheckAndUseDistance:
      analogWrite(Pw, MinimumSpeed);
      while(CheckAmount > 0){
        long DurationF;
        digitalWrite(TriggerF, LOW);
        delayMicroseconds(2);
        digitalWrite(TriggerF, HIGH);
        delayMicroseconds(10);
        digitalWrite(TriggerF, LOW);
        DurationF = pulseIn(EchoF, HIGH);
        DistanceF = (DurationF / 2) / 29.1;
  
        long DurationL;
        digitalWrite(TriggerL, LOW);
        delayMicroseconds(2);
        digitalWrite(TriggerL, HIGH);
        delayMicroseconds(10);
        digitalWrite(TriggerL, LOW);
        DurationL = pulseIn(EchoL, HIGH);
        DistanceL = (DurationL / 2) / 29.1;

        AllDistancesL += DistanceL;
        AllDistancesF += DistanceF;
        CheckAmount--;
        EState = CheckAndUseDistance;
      }
      CheckAmount = 100;
      DistanceL = AllDistancesL / CheckAmount;
      DistanceF = AllDistancesF / CheckAmount;
      AllDistancesL = 0;
      AllDistancesF = 0;
      /*
      if(0 < RunTimeAmount){
        if(DistanceF < (SweetSpotFLow - AdjustingForTurnDistance)){
          //client.publish("mess", "Kör bakåt för att distansen frammåt är ");
          EState = GetFurtherF;
        }else if(DistanceF > (SweetSpotFHigh - AdjustingForTurnDistance)){
          //client.publish("mess", "Kör fram för att distansen frammåt är ");
          EState = GetCloserF;
        }else{
          EState = Still;
        }
      }else if(StartSteg == 3){
        RunTimeAmount = 5;
        AdjustingForTurnDistance = 0;
        digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
        analogWrite(Pw, Acc);
        //client.publish("mess", "Hittade väggen, kör fram för att corrigera vinkeln");
        delay(2000);
        analogWrite(Pw, MinimumSpeed);
        EState = CheckAndUseDistance;
      }else if(StartSteg == 2){
        digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
        Degree = DegreeMax;
        StartSteg--;
        EState = Turn;
      }
      */
      previousMillis = millis();
      if (DistanceL > SweetSpotL) {        
        Degree = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      } else if (DistanceF >= SweetSpotFLow && DistanceF <= SweetSpotFHigh) {        
        Degree = DegreeMax;
        EState = Turn;
      } else if (DistanceF < SweetSpotFLow) {
        DriveDistance = (SweetSpotFLow -= DistanceF);
        DriveTime = DriveDistance * DriveDistanceGGR;
        //client.publish("mess", "Backar " + DriveTime + " ms för att distansen frammåt är " + DistanceF + " cm");
        EState = GetFurtherF;
      } else if (DistanceF > SweetSpotFHigh) {
        DriveDistance = (DistanceF -= SweetSpotFHigh);
        DriveTime = DriveDistance * DriveDistanceGGR;
        //client.publish("mess", "Kör fram " + DriveTime + " ms för att distansen frammåt är " + DistanceF + " cm");     
        EState = GetCloserF;
      }
      break;

    case GetCloserF:
      digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
      analogWrite(Pw, Acc);
      currentMillis = millis();
      if (currentMillis - previousMillis >= DriveTime){
        previousMillis = currentMillis;
        Degree = DegreeMitten;
        My_servo.write(Degree);
        AllDistancesL = 0;
        CheckAmount2 = 10;
        //client.publish("mess", "Stannar och kollar värdern");
        EState = CheckAndUseDistance;
      }
      long DurationL;
        oldDistanceL = DistanceL;
        if(CheckAmount2 > 0){
          digitalWrite(TriggerL, LOW);
          delayMicroseconds(2);
          digitalWrite(TriggerL, HIGH);
          delayMicroseconds(10);
          digitalWrite(TriggerL, LOW);
          DurationL = pulseIn(EchoL, HIGH);
          DistanceL2 = (DurationL / 2) / 29.1;
          AllDistancesL += DistanceL2;
          CheckAmount2--;
          EState = GetCloserF;
        }
        CheckAmount2 = 10;
        DistanceL2 = AllDistancesL/10;
        AllDistancesL = 0;
        if(DistanceL2 > SweetSpotL){
          Degree = DegreeMinimum;
          turnDelay = 3200;
          analogWrite(Pw, MinimumSpeed);
          //client.publish("mess", "Valde att svänga vänster för att distansen vänster är " + DistanceL2);
          EState = Turn;
        }else if(DistanceL2 < oldDistanceL){
          Degree += 1;
          My_servo.write(Degree);
          DistanceL = DistanceL2;
          EState = GetCloserF;
        }else if(DistanceL2 > oldDistanceL){
          Degree -= 1;
          My_servo.write(Degree);
          DistanceL = DistanceL2;
          EState = GetCloserF;
        } else {
          DistanceL = DistanceL2;
          EState = GetCloserF;
        }
    break;

    case GetFurtherF:
      digitalWrite(D1, LOW), digitalWrite(D2, HIGH);
      analogWrite(Pw, Acc);
      delay(DriveTime);
      EState = CheckAndUseDistance;
      //client.publish("mess", "Stannar och kollar värdern");
    break;

    case Turn:
      digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
      if(StartSteg == (0)){
        if (Degree == DegreeMinimum) {
          Direction = "vänster";
          //client.publish("mess", "Svänger " + Direction + " för att distansen vänster är " + DistanceL);
        } else {
          Direction = "höger";
          //client.publish("mess", "Svänger " + Direction + " för att distansen frammåt är " + DistanceF);
        }
      }
      My_servo.write(Degree);
      delay(1000);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      analogWrite(Pw, MinimumSpeed);
      if(StartSteg == 1){
        StartSteg--;
        //client.publish("mess", "I position");
      }
      //client.publish("mess", "Stannar och kollar värdern");
      delay(500);
      Degree = DegreeMitten;
      My_servo.write(Degree);
      delay(1000);
      turnDelay = 3450;
      EState = CheckAndUseDistance;
    break;

    case Still:
      RunTimeAmount--;
      if (RunTimeAmount == 1) {
        StartSteg--;
      }
      analogWrite(Pw, MinimumSpeed);
      EState = CheckAndUseDistance;
    break;
  }
}
/*
  void onConnectionEstablished() {
  //client.subscribe("lampa/lamp", [](const String & payload){
    IsConnected = true;
    //client.publish("mess", "Har startat");
    EState = StartUp;
  });
  }
*/
