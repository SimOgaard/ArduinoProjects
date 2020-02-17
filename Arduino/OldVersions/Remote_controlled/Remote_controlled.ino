#include "EspMQTTClient.h"
#include <Servo.h>

typedef enum EngineState {
  Execute, Recive,
};
EngineState EState;

void onConnectionEstablished();

EspMQTTClient client(
 "ABBIndgymIoT_2.4GHz",           // Wifi ssid
  "ValkommenHit!",           // Wifi password
  "192.168.0.120",  // MQTT broker ip
  1883,             // MQTT broker port
  "jocke",            // MQTT username
  "apa",       // MQTT password
  "microdator",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages
);

#define TriggerL 12
#define EchoL 14
#define TriggerF 2
#define EchoF 13
#define D1 0
#define D2 4
#define Pw 5

bool NewCommand = false;
bool getFront;
bool getLeft;

int Degree;
int Speed;
int Delay;

int DistanceL;
int DistanceF;

void setup() {
  My_servo.attach(15);
  Serial.begin(9600);
  pinMode(TriggerL, OUTPUT), pinMode(EchoL, INPUT);
  pinMode(TriggerF, OUTPUT), pinMode(EchoF, INPUT);
  pinMode(Pw, OUTPUT), pinMode(D1, OUTPUT), pinMode(D2, OUTPUT);
}

void loop() {
  switch (EState) {
    case Recive:
      client.loop();

      //recive payload, that is: BOOL:NewCommand, BOOL: getFront, BOOL: getLeft, INT: Degree, INT: Speed, INT Delay

      if(NewCommand == true){
        NewCommand = false;
        EState = Execute;
      }
      EState = Recive;
    break;
    
    case Execute:
      if(getFront == true) {
        long DurationF;
        digitalWrite(TriggerF, LOW);
        delayMicroseconds(2);
        digitalWrite(TriggerF, HIGH);
        delayMicroseconds(10);
        digitalWrite(TriggerF, LOW);
        DurationF = pulseIn(EchoF, HIGH);
        DistanceF = (DurationF / 2) / 29.1;
        client.publish("mess", "The distance forward is " + DistanceF + " cm");
      }
      if(getLeft == true){
        long DurationL;
        digitalWrite(TriggerL, LOW);
        delayMicroseconds(2);
        digitalWrite(TriggerL, HIGH);
        delayMicroseconds(10);
        digitalWrite(TriggerL, LOW);
        DurationL = pulseIn(EchoL, HIGH);
        DistanceL = (DurationL / 2) / 29.1;
        client.publish("mess", "The distance left is " + DistanceL + " cm");
      }
      My_servo.write(Degree)
      delay(350);
      analogWrite(Pw, Speed);
      delay(Delay);
      analogWrite(Pw, 0);
      EState = TakeCommand;
    break;
}

void onConnectionEstablished() {
  client.subscribe("lampa/lamp", [](const String & payload) {
    Serial.println(payload);
  });
}
