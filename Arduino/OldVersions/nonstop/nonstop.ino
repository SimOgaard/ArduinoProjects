#include <Servo.h>
typedef enum EngineState {
  StartUp, Turn, Drive, CheckAndUseDistance, CheckDistance, UseDistance,
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
const int DegreeMinimum = 0;
const int DegreeMitten = 64;
const int DegreeMax = 125;
int turnDelay = 3450;
int TurnAmount;
int TurnAmountStart = 50;
int Degree;
int DegreeAcc = 1;
int ButtonState = 0;
int StartSleep = 1500;
int DistanceL;
int DistanceF;
int DistanceL2 = 0;
int AllDistancesF;
int AllDistancesL;
int CheckAmountStart = 10;
int CheckAmount;
const int Acc = 700;
int SweetSpotL = 22;
int SweetSpotFHigh = 14;
int SweetSpotFLow = 13;

int arraysize = 5;  //quantity of values to find the median (sample size). Needs to be an odd number
int rangevalue[] = {0, 0, 0, 0, 0};    //declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer

Servo My_servo;
void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(ButtonPin, INPUT);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT), pinMode(D2, OUTPUT);
  CheckAmount = CheckAmountStart;
  EState = StartUp;
}
void loop() {
  switch (EState) {
    case StartUp:
      ButtonState = digitalRead(ButtonPin);
      Degree = DegreeMinimum;
      My_servo.write(Degree);
      if (ButtonState == LOW) {
        Degree = DegreeMitten;
        My_servo.write(Degree);
        delay(StartSleep);
        EState = CheckDistance;
      } else {
        EState = StartUp;
      }
    break;
    case CheckAndUseDistance:
      //if(CheckAmount <= 0){
       /* long DurationF;
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
        *//*
        AllDistancesF += DistanceF;
        AllDistancesL += DistanceL;
        CheckAmount--;
       
        if(DistanceL == 0 || DistanceF == 0){
          Serial.println("SENSOR IS NOT WORKING");
          Serial.println(String(DistanceL) + String("    ") + String(DistanceF));
          EState = CheckAndUseDistance;
        } else {
          Serial.println(String(DistanceL) + String("    ") + String(DistanceF));
          Serial.println("SENSOR IS WORKING");
        }
        
        EState = CheckAndUseDistance;
      }
      Serial.println(String(AllDistancesL) + String("    ") + String(AllDistancesF));
      CheckAmount = CheckAmountStart;
      DistanceF = AllDistancesF / CheckAmount;
      DistanceL = AllDistancesL / CheckAmount;
      AllDistancesF = 0;
      AllDistancesL = 0;
      */
      Serial.println(DistanceL + String("    ") + DistanceF);
      /*if (DistanceL > SweetSpotL) {
        Degree = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      }*/
      Serial.println(CheckAmount);
      if(DistanceL > SweetSpotL && CheckAmount > 0){
        CheckAmount--;
        EState = CheckAndUseDistance;
      } else if (DistanceL > SweetSpotL){
        CheckAmount = CheckAmountStart;
        Degree = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      } else{
        CheckAmount = CheckAmountStart;
      }
      if (DistanceF <= SweetSpotFHigh && DistanceF >= SweetSpotFLow) {        
        Degree = DegreeMax;
        turnDelay = 3450;
        EState = Turn;
      } else if (DistanceF < SweetSpotFLow) {
        digitalWrite(D1, LOW), digitalWrite(D2, HIGH);
        EState = Drive;
      } else if (DistanceF > SweetSpotFHigh) {   
        digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
        EState = Drive;
      }
    break;
    case CheckDistance:
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
      Serial.println(DistanceL + String("    ") + DistanceF);


      AverageF += DistanceF;
      AvarageF 

      
      EState = UseDistance;
    break;
    case UseDistance:
      if (DistanceL > SweetSpotL) {
        Degree = DegreeMinimum;
        turnDelay = 3200;
        EState = Turn;
      } else if (DistanceF <= SweetSpotFHigh && DistanceF >= SweetSpotFLow) {        
        Degree = DegreeMax;
        turnDelay = 3450;
        EState = Turn;
      } else if (DistanceF < SweetSpotFLow) {
        digitalWrite(D1, LOW), digitalWrite(D2, HIGH);
        EState = Drive;
      } else if (DistanceF > SweetSpotFHigh) {   
        digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
        EState = Drive;
      }
    break;
    case Drive:
      analogWrite(Pw, Acc);
      if(TurnAmount <= 0){
        if(DistanceL2 != 0){
          if(DistanceL2 > DistanceL){
            Degree+=DegreeAcc;
            My_servo.write(Degree);
          }else if(DistanceL2 < DistanceL){
            Degree-=DegreeAcc;
            My_servo.write(Degree);
          }
        }
        TurnAmount = TurnAmountStart;
        DistanceL2 = DistanceL;
      }
      TurnAmount--;
      EState = CheckDistance;
    break;
    case Turn:
      digitalWrite(D1, HIGH), digitalWrite(D2, LOW);
      My_servo.write(Degree);
      analogWrite(Pw, Acc);
      delay(turnDelay);
      Degree = DegreeMitten;
      My_servo.write(Degree);
      EState = CheckDistance;
    break;
  }
}

for(int i = 0; i < arraysize; i++)
  {                                                    
    rangevalue[i] =
  }
    isort(rangevalue, arraysize);
    int midpoint = arraysize/2;
    
void isort(int *a, int n)
{
 for (int i = 1; i < n; ++i)
 {
   int j = a[i];
   int k;
   for (k = i - 1; (k >= 0) && (j < a[k]); k--)
   {
     a[k + 1] = a[k];
   }
   a[k + 1] = j;
 }
}

void printArray(int *a, int n)
{
 
 for (int i = 0; i < n; i++)
 {
   Serial.print(a[i], DEC);
   Serial.print(' ');
 }
 
 Serial.println();
}
