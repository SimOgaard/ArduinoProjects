int Sensor = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(Sensor));
  delay(100);
}
