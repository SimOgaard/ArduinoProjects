#define AI_Pot 0 //input till AD
#define DO_RLed 14 //output till D5

int PotValue = 0; //sätter startvärdet för potvalue till 0

void setup() { //setuppen körs en gång
  pinMode(DO_RLed, OUTPUT); //sätter iopinnen till utgång
  pinMode(AI_Pot, INPUT); //sätter iopinnen till ingång
  Serial.begin(9600); //sätter datavärdet i bits per sekund
}

void loop() { //körs hela tiden
  PotValue = analogRead(AI_Pot); //läser av potensmätaren
  analogWrite(DO_RLed, PotValue); //HIGH är på, potvalue är intervall 0-1023 av till på
  Serial.println(PotValue); //printar det avlästa värdet
  delay(50);//väntar 50 milisekunder 
}
