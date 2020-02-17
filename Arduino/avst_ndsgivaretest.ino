typedef enum HandStates{
  Hand,
  Hand2,
  NoHand,
  NoHand2,
  Foot,
  Foot2,
  NoFoot,
  NoFoot2,
};

HandStates HState;


#define AI_Pot 0
#define DO_RLed 14

int PotValue = 75;

void setup() {
  pinMode(DO_RLed, OUTPUT);
  pinMode(AI_Pot, INPUT);
  Serial.begin(9600);
  analogWrite(DO_RLed, PotValue);
  HState = NoFoot;
}

void loop() {
  switch (HState){

    case NoFoot:
      PotValue = analogRead(AI_Pot);
      Serial.println(PotValue);
      delay(100);
    if ( (50 < PotValue) && (PotValue < 100) ) {
      HState = Foot;
    } else { //skicka speciella värden vidare sedan till appen och använd dem för att veta vad dem gör, och för att göra en how to "video"
      HState = NoHand;
    }
  break;

    case Foot:
      PotValue = analogRead(AI_Pot);
      Serial.println(PotValue);
      delay(100);
    if ( PotValue < 50 ) { //kanske byt plats på if och else
      HState = NoFoot2;
    } else{
      Serial.print("fb");
      HState = Foot;
    }
  break;

    case NoFoot2:
      for (int w=0; w <= 25; w++){
        PotValue = analogRead(AI_Pot);
        Serial.println(PotValue);
        delay(100);
      if ( (15 < PotValue) && (PotValue < 50) ) {
        HState = Foot2;
        break;
      } else {
        Serial.print("fc");
        }
      }
    if (15 < PotValue < 50){
    HState=NoFoot;
    }
  break;

    case Foot2:
      analogWrite(DO_RLed, LOW);
      delay(250);
      HState = NoFoot;
  break;
  
    case NoHand:
      PotValue = analogRead(AI_Pot);
      Serial.print("a");
      Serial.println(PotValue);
      delay(100);
    if (PotValue > 50)  {
      HState = Hand;
    } else{
      Serial.print("a");
      HState = NoFoot;
    }
  break;
  
    case Hand:
      PotValue = analogRead(AI_Pot);
      Serial.println(PotValue);
      delay(100);
    if (PotValue < 50)  {
      HState = NoHand2;
    } else {
      Serial.print("b");
      HState = Hand;
      }
    break;

    case NoHand2:
      for (int q=0; q <= 50; q++){
        PotValue = analogRead(AI_Pot);
        Serial.println(PotValue);
        delay(100);
      if (PotValue > 50) {
        HState = Hand2;
        break;
      } else {
        Serial.print("c");
        }
      }
    if (PotValue < 50){
      HState = NoHand;
    }
  break;

    case Hand2:
      PotValue = analogRead(AI_Pot);
      Serial.println(PotValue);
      if (PotValue < 50) {
        HState=NoHand;
      } 
      else {
      Serial.print("d");
      analogWrite(DO_RLed, PotValue);
      delay(100);
      HState = Hand2;
      }
  break;
  }
}
