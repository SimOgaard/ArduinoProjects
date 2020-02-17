typedef enum TrafficLightStates{ //custom data typer, och enum krävs när man använder led, det är en int
  StateRed, //skapar datatypen statered
  StateRedYellow, //skapar datatypen stateredyellow
  StateGreen, //skapar datatypen stategreen
  StateYellow, //skapar datatypen stateyellow
  StateYellow2, //skapar datatypen stateyellow2
};

TrafficLightStates TLState; //variabel, värderna ovan

#define DI_CarSensor 15 //InputD8 on NodeMCU
#define DO_RLed 14 //Output D5 on NodeMCU
#define DO_YLed 12 //Output D6 on NodeMCU
#define DO_GLed 13 //Output D7 on NodeMCU

bool buttonState = 0; //håller en eller två värden, sant falskt, kan ändras

void setup() { //körs 1 gång
  pinMode(DO_RLed, OUTPUT); //säger åt (DO_RLed) att antingen vara en output eller input
  pinMode(DO_YLed, OUTPUT);
  pinMode(DO_GLed, OUTPUT);
  pinMode(DI_CarSensor, INPUT);
}

void loop() { //loop som fortsätter hela tiden
  buttonState = digitalRead(DI_CarSensor); //läser av InputD8 värde, är den san/falsk
    
  switch (TLState){ //kontrolerar flödet på koden säger åt att de spesefika koderna innom en case som körs tills ett break blir utförd
    case StateRed:  //detta är den spesefika koden
      digitalWrite(DO_RLed, HIGH); //tända den röda lampan
      digitalWrite(DO_YLed, LOW);
  
    if (buttonState == HIGH)  { //testar om knappen trycks
        TLState = StateRedYellow; // om den gör det ändrar den tillstånd
      } else{
        TLState = StateRed; //annars fortsätter den vid detta tillstånd
      }
    break;  //gå ur case
  
    case StateRedYellow:
      digitalWrite(DO_YLed, HIGH);
      digitalWrite(DO_RLed, HIGH);
      delay(2500); 
      digitalWrite(DO_YLed, LOW); //släcker gul lampa
      digitalWrite(DO_RLed, LOW);
      TLState = StateGreen;
    break;
  
    case StateGreen: 
      digitalWrite(DO_GLed, HIGH); 
      delay(3000);
      digitalWrite(DO_GLed, LOW);
      TLState = StateYellow;
    break;
  
    case StateYellow: 
      digitalWrite(DO_YLed, HIGH); // DETTA ÄR ETT TEST
      uint32_t period = 2000L;

      for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
        if (buttonState == HIGH) {
          TLState = StateGreen;
          digitalWrite(DO_YLed, LOW);
          } else{
            TLState = StateRed;
          }
        }
    break;
    }
  }


