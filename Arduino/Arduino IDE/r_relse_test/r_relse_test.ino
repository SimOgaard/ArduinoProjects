#define sensor 15
int x = 0;
int y = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
x = analogRead(sensor);
Serial.println(x); 
}
