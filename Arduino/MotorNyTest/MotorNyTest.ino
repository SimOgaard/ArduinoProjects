typedef enum EngineStates {
  Starting,
  Forward,
  Backward,
  Stopping,
  Switching,
};

EngineStates EState;

#define D1 12
#define D2 13
#define Pw 14

int Speed;

int Acc;

//bool GoingF;
//bool GoingB;

bool AllowAccF;
bool AllowAccB;

void setup() {
  Serial.begin(9600);
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  //Speed = 300;
  //Acc = 5;

  //EState = Starting;
}

void loop() {
  switch (EState) {

    case Forwards:
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
  
//      GoingForwards = true;
//      GoingBackwards = false;
    break;

    case Backwards:
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      
//      GoingForwards = false;
//      GoingBackwards = true;
    break;

    case SpeedUp:

      if (AllowAccB == true) {
        Speed += 1;
        analogWrite(Pw, Speed);
        Serial.println(Speed);
      }
      
      CheckSpeed();
    break;

    case SloowDown:

      
      
    break;

    case Switching:
      
    break;
  }
}

void CheckSpeed() {
  if (Speed > 0 && Speed < 1023) {
    AllowAccF = true;
    AllowAccB = true;
  } else if (Speed = 1023) {
    AllowAccF = false;
  } else if (Speed = 0) {
    AllowAccB = false;
  }
}
