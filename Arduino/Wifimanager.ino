#define D7 13

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

void setup() {
  pinMode(13,OUTPUT);
  
  Serial.begin(115200);
  WiFiManager wifiManager;
  
  //wifiManager.resetSettings();
  
  //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  wifiManager.autoConnect("SimonAutoConnectAP");
  
  //wifiManager.autoConnect();
  
  Serial.println("connected...yeey :)");
}

String Lampname="hej";
int Hardvalue=0;
int Strengthvalue=0;
bool LampExist=false;
bool GottenValues=false;


String GetfromDB(String host){
  String url= "/light/"+Lampname; 
  String Output ="GET "+ url + " HTTP/1.1\r\n" + "Host: " + host+ "\r\n" + "\r\nConnection: close\r\n\r\n";
  return Output;
}

String SendtoDB(String host){
  String type="POST ";
  if(GottenValues==true) {
    String url="/light/";
    //Serial.println("Skickar värde första gången");
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["name"] = Lampname;
    root["hard"] = Hardvalue;
    root["strength"] = Strengthvalue;
    String buffer;
    root.printTo(buffer);
    if(LampExist==true){
      type ="PATCH ";
      Serial.println("Uppdaterar värdet");
    }
    String Output =type+url+"HTTP/1.1\r\n"+"Host: "+ host+"\r\n"+"Content-Type: application/json\r\n"+"Content-Length: " + buffer.length() + "\r\n"+"\r\n"+buffer + "\n";
    return Output;
  }
  else
  return "";
}
      
void ConnecttoDB(String input){
  const int httpPort=3000;
  const char*host="192.168.1.2";
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  if(!client.connect(host, httpPort)){
    Serial.println("connection failed");
    return;
  } else {
    //digitalWrite(13,HIGH);
  }
  if(input =="GET")
    client.print(GetfromDB(host));
  else
    client.print(SendtoDB(host));
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  String json = "";
  boolean httpBody = false;
    
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if (!httpBody && line.charAt(1) == '{') {
      httpBody = true;
    }
    if (httpBody) {
      json += line;
    }
  }

  Serial.println("Got data:");
  Serial.println(json);
  if(input =="GET")
    UpdateValues(json);
  Serial.println();
  Serial.println("closing connection");
}

void UpdateValues(String json){
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  String dataL = root["name"];
    if(dataL!="none") {
      int datah = root["hard"];
      int datas = root["strength"];
      Lampname = dataL;
      Hardvalue = datah;
      Strengthvalue = datas;
      LampExist=true;
      Serial.print(Strengthvalue);
    } else{
      String Mess = root["message"];
      Serial.print(Mess);
    }
  GottenValues=true;
}

void UpdatingLamp(){
  analogWrite(13, Strengthvalue);
}

void loop() {
  ConnecttoDB("GET");
  UpdatingLamp();
  delay(1000);
  ConnecttoDB("POST");
  delay(1000);
}
