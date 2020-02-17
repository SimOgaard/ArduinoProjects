#define D7 13
//#define DO_RLed 14 //varför använder vi 13 och inte 14 

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>

String Lampname = "Lampgod";
int Hardvalue = 0;
int Strengthvalue = 0;
bool LampExist = false;
bool GottenValues = false;

    void ConnecttoDB(String input){

      const int httpPort=3000;
      const char*host="192.168.0.102";

      Serial.print("connecting to");
      Serial.println(host);

      WiFiClient client;
      if(!client.connect(host,httpPort)){
      Serial.println("connection failed");
      return;
      } else {
      digitalWrite(13,HIGH);
      }

    Serial.println();
    Serial.println("closing connection");

    if (input == "GET") {
      client.print(GetfromDB(host));
    } else {
      client.print(SendtoDB(host));
      unsigned long timeout=millis();
      while(client.available()==0) {
        if(millis()-timeout>10000){
          Serial.println(">>>Client Timeout!");
          client.stop();
          return;
        }
      }
      String json="";
      boolean httpBody=false;
      while (client.available()){
        String line = client.readStringUntil('\r');
        if(!httpBody && line.charAt(1)=='{'){
          httpBody=true;
        }
        if (httpBody) {
          json+=line;
        }
      }
      Serial.println("Got data:");
      Serial.println(json);
      if (input=="GET") {
        UpdateValues(json);
        Serial.println();
        Serial.println("closing connection");
      }
    }
  }
    
String SendtoDB(String host){
  String type="POST";
  if(GottenValues==true) {
    String url="/light/";
      Serial.println("Skickar värde första gången");
      StaticJsonBuffer<300>jsonBuffer;
      JsonObject&root=jsonBuffer.createObject();
      root["name"]=Lampname;
      root["hard"]=Hardvalue;
      root["strength"]=Strengthvalue;
      String buffer;
      root.printTo(buffer);
      if(LampExist==true){
        type="PATCH";
        Serial.println("Uppdaterar värdet");
      }
      String Output=type+url+"HTTP/1.1\r\n"+
      "Host:"+host+"\r\n"+
      "Content-Type:application/json\r\n"+
      "Content-Length:"+buffer.length()+"\r\n"+
      "\r\n"+
      buffer+"\n";
      return Output;
  }
  else
  return"";
}

String GetfromDB(String host){
  String url="/light/"+Lampname;
  String Output="GET"+url+"HTTP/1.1\r\n"+
  "Host:"+host+"\r\n"+
  "\r\nConnection:close\r\n\r\n";
  return Output;
}

void UpdateValues(String json){
  StaticJsonBuffer<400>jsonBuffer;
  JsonObject&root=jsonBuffer.parseObject(json);
  String dataL=root["name"];
    if(dataL!="none") {
      int datah=root["hard"];
      int datas=root["strength"];
      Lampname=dataL;
      Hardvalue=datah;
      Strengthvalue=datas;
      LampExist=true;
    } else{
      String Mess=root["message"];
      Serial.print(Mess);
    }
  GottenValues=true;
}

void UpdatingLamp(){
  if(Strengthvalue>50){
    digitalWrite(13,HIGH);
  } else{
    digitalWrite(13, LOW);
  }
}

void setup() {
    pinMode(13,OUTPUT);
    
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

void loop() {
    ConnecttoDB("GET");
    UpdatingLamp();
    delay(10000);
}
