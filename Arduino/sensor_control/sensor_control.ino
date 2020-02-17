#define TRIG_1 14
#define ECHO_1 4

#define TRIG_2 13
#define ECHO_2 15

void setup() {
  Serial.begin (9600);
  
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);
}

void loop() {
  dist_fram();
  delay(2000);
  dist_right();
}

long dist_fram(){
  long duration, dist_1;
  digitalWrite(TRIG_1, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_1, LOW);
  duration = pulseIn(ECHO_1, HIGH);
  dist_1 = (duration/2) / 29.1;
  Serial.print("Centimeter_Fram: ");
  Serial.println(dist_1);
  return dist_1;
}

long dist_right (){
  long duration, dist_2;
  digitalWrite(TRIG_2, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_2, LOW);
  duration = pulseIn(ECHO_2, HIGH);
  dist_2 = (duration/2) / 29.1;
  Serial.print("Centimeter_Sida: ");
  Serial.println(dist_2);
  return dist_2;
}
