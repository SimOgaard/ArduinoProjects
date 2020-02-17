#define TriggerL 12
#define EchoL 14
#define TriggerF 2
#define EchoF 13

int DistanceL, DistanceF;
int MinstaL, MinstaF, HogstaL, HogstaF, AllDistanceL, AllDistanceF;

void setup() {
  Serial.begin(9600);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
}

void loop() {
      MinstaL = 9999;
      MinstaF = 9999;
      HogstaL = 0;
      HogstaF = 0;
      AllDistanceL = 0;
      AllDistanceF = 0;
      for(int i = 0; i < 5; i++){

  long DurationL;
  digitalWrite(TriggerL, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerL, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerL, LOW);
  DurationL = pulseIn(EchoL, HIGH);
  DistanceL = (DurationL / 2) / 29.1;
  long DurationF;
  digitalWrite(TriggerF, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerF, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerF, LOW);
  DurationF = pulseIn(EchoF, HIGH);
  DistanceF = (DurationF / 2) / 29.1;

        Serial.println(DistanceL + String("     ") + DistanceF);
        AllDistanceL += DistanceL;
        AllDistanceF += DistanceF;
        if(DistanceL < MinstaL){
          MinstaL = DistanceL;
        }
        if (DistanceL > HogstaL){
          HogstaL = DistanceL;
        }
        if(DistanceF < MinstaF){
          MinstaF = DistanceF;
        }
        if (DistanceF > HogstaF){
          HogstaF = DistanceF;
        }
      }
      AllDistanceL -= (MinstaL + HogstaL);
      AllDistanceF -= (MinstaF + HogstaF);
      DistanceL = AllDistanceL / 3;
      DistanceF = AllDistanceF / 3;

      Serial.println(String("LEFT: ") + DistanceL + String("     FORWARD: ") + DistanceF);
      Serial.println(String("MINLEFT: ") + MinstaL + String(" MINFORWARD: ") + MinstaF + String(" HIGHLEFT: ") + HogstaL + String(" HIGHFORWARD: ") + HogstaF);
}
