typedef enum EngineState{
  Forwards,
  Backwards,
  Stopping,
  STOP,
  SwitchingForwardsToBackwards,
};

EngineState EState;

#define D1 12
#define D2 13
#define Pw 14

int Speed;

int ChangeOne;
int ChangeTwo;
int ChangeThree;
int ChangeFour;

bool GoingForwards;
bool GoingBackwards;
bool AllowAccF;
bool AllowAccB;

void setup() {
  Serial.begin(9600);
  
  pinMode(Pw, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  Speed = 1;
  CheckSpeed();
  EState = Forwards;
}

void loop() {

  switch (EState) {
  
  case Forwards:
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);

    GoingForwards = true;
    GoingBackwards = false;
    
    if (GoingForwards == true && AllowAccF == true) {
      Speed += 1;
      analogWrite(Pw, Speed);
      Serial.println(Speed);
    }

    CheckSpeed();
  break;

  case SwitchingForwardsToBackwards:
    if (Speed > 0) {
      Speed -= 1;
      analogWrite(Pw, Speed);
    } else if (GoingForwards == true && Speed <= 0) {
      EState = Backwards;
    } else if (GoingBackwards == true && Speed <= 0) {
      EState = Forwards;
    }

    CheckSpeed();

  break;
  
  case Backwards:
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    
    GoingForwards = false;
    GoingBackwards = true;

    if (GoingBackwards == true && AllowAccB == true) {
      Speed += 1;
      analogWrite(Pw, Speed);
      Serial.println(Speed);
    }
    
    CheckSpeed();

  break;

  case Stopping:
    if (Speed > 0) {
      Speed -= 1;
      analogWrite(Pw, Speed);
    }
    
    CheckSpeed();

  break; 
  
  case STOP:
    Speed = 1;
    analogWrite(Pw, Speed);

    CheckSpeed();

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
