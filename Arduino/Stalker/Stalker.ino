typedef enum EngineState{
  Distance,
  Forwards,
  Backwards,
};

EngineState EState;

#define TRIGGER 2
#define ECHO 0

#define D1 12
#define D2 13
#define Pw 14

int minimum = 20;
int maximum = 30;

bool AllowAccF;
bool AllowAccB;

int Speed;

void setup() {
  Serial.begin(9600);
  
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  Speed = 0;
}

void loop() {
  switch (EState) {
    
    case Distance:
      long duration, distance;
      digitalWrite(TRIGGER, LOW);  
      delayMicroseconds(2); 
      digitalWrite(TRIGGER, HIGH);
      delayMicroseconds(10); 
      digitalWrite(TRIGGER, LOW);
      duration = pulseIn(ECHO, HIGH);
      distance = (duration/2) / 29.1;
      Serial.print("Centimeter: ");
      Serial.println(distance);
      
      if (Speed >= 1023) {
        AllowAccF = false;
        AllowAccB = true;
      } else if (Speed <= 0) {
        AllowAccF = true;
        AllowAccB = false;
      }
      if (distance <= minimum) {
        EState = Backwards;
      } else if (distance >= maximum) {
        EState = Forwards;
      } else {
        analogWrite(Pw, 0);
      }
    break;
  
    case Forwards:
      if(AllowAccF == true) {
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        
        if (AllowAccF == true) {
          Speed += 1;
          analogWrite(Pw, Speed);
          Serial.println(Speed);
        }
        
        EState = Distance;
      } else {
        EState = Distance;
      }
    break;
  
    case Backwards:
      if(AllowAccB == true) {
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);

        if (AllowAccB == true) {
          Speed += 1;
          analogWrite(Pw, Speed);
          Serial.println(Speed);
        }
  
        analogWrite(Pw, Speed);
  
        EState = Distance;
      }
    break;
  }
}
