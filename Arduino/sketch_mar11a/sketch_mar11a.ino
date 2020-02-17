#include "EspMQTTClient.h"

void onConnectionEstablished();

EspMQTTClient client(
 "ABBIndgymIoT_2.4GHz",           // Wifi ssid
  "ValkommenHit!",           // Wifi password
  "192.168.0.115",  // MQTT broker ip
  1883,             // MQTT broker port
  "jocke",            // MQTT username
  "apa",       // MQTT password
  "microdator",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages
);
#define led_pin D1


void setup() {
pinMode(led_pin, OUTPUT);
digitalWrite(led_pin,LOW);
Serial.begin(115200);
}

bool off=false;

void lampa(){
  if(off==true)
  {
  Serial.println("Släckt!");
  off=false;
  }
  else
  {
  off=true;
  
  Serial.println("Släckt!");
  }

digitalWrite(led_pin,off);
  
}

void onConnectionEstablished()
{
  client.subscribe("lampa/lamp", [] (const String &payload)
  {
    Serial.println(payload);
//    if(payload=="on")
  //  onlampa();
    lampa();
  });
  
  client.publish("mess", "This is a message");

  client.executeDelayed(5 * 1000, []() {
    client.publish("mess", "This is a message sent 5 seconds later");
  });
}


void loop() {
  // put your main code here, to run repeatedly:
client.loop();
}
