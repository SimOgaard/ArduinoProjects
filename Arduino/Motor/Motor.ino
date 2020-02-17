typedef enum DriveStates {
  Starting,
  Stopping,
  Forward,
  Backward,
};

DriveStates DState;

byte PotInput = 2; //WHAT
int PotValue;
byte SwitchInput = 6; //WHAT
boolean SwitchOn;
byte Speed; //WHAT

byte Pwm_b = 12;
byte Dir_b1 = 13;
byte Dir_b2 = 14;

byte RedLed = 3;
byte YellowLed = 4;

void setup() {
  Serial.begin(9600);
  pinMode(SwitchInput, INPUT);

  pinMode(RedLed, OUTPUT);
  pinMode(YellowLed, OUTPUT);

  pinMode(Pwm_b, OUTPUT);
  pinMode(Dir_b1, OUTPUT);
  pinMode(Dir_b2, OUTPUT);

  analogWrite(Pwm_b, 0);
  DState = Starting;
}

void loop() {
  PotValue = analogRead(PotInput);
  Speed=map(PotValue,0,1023,0,255);
  SwitchOn = digitalRead(SwitchInput);

  switch (DState) {
    case Starting:
      digitalWrite(YellowLed, HIGH);
      digitalWrite(RedLed, HIGH);
      delay(3000);
      DState = Stopping;
    break;
    
    case Stopping:
      if (!SwitchOn){
        DState = Forward;
      } else {
        DState = Backward;
      }
    break;

    case Forward:
      digitalWrite(YellowLed, HIGH);
      digitalWrite(RedLed, LOW);
      digitalWrite(Dir_b1, LOW);
      digitalWrite(Dir_b2, HIGH);
    break;
    
    case Backward:
      digitalWrite(YellowLed, LOW);
      digitalWrite(RedLed, HIGH);
      digitalWrite(Dir_b1, HIGH);
      digitalWrite(Dir_b2, LOW);
    break;
  }
}
