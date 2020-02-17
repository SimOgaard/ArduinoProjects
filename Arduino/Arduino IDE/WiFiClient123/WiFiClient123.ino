#include <ESP8266WiFi.h>
#include <Wire.h> // används för kunna ta emot från givare
#include <AM2320.h> // bilboltek för givaren

AM2320 sensor; // anger att vi har en sensor av typen AM2320

const char* ssid     = "ABB_Indgym_Guest";
const char* password = "Welcome2abb";

const char* host = "193.41.215.179";

int sensorHeat= 50; 
int sensorHum = 50;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(12,13); // bestämmer vart givaren ska kopplas in
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendtemperature(int temp, int hum){
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // create url
  int id = 72;
  String url = "/termin1/grupptre/Simon/reciver.php?";
  url += "Id=";
  url += id;
  url += "&Tempratur=";
  url += temp;
  url+= "&Fukt=";
  url+= hum;


  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  
  } 
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");



}

// funktion för att få temperatur.
void getTemperature() {
  if (sensor.measure()){
    Serial.print("Temperature: ");
    Serial.println(sensor.getTemperature());
    sensorHeat=sensor.getTemperature(); // ger sensorHeat tempratuyren som sensorn har mätt
    sensorHum=sensor.getHumidity(); // ger sensorHum luftfuktighet som sensorn har mätt
    Serial.print("Humidity: ");
    Serial.println(sensor.getHumidity());
    sendtemperature(sensorHeat, sensorHum);
    
  }
  else {
    int errorCode = sensor.getErrorCode();
    switch (errorCode){
      case 1: Serial.println("ERR: Sensor is offline"); break;
      case 2: Serial.println("ERR: CRC validation failed."); break;
    }
  }
}





void loop() {
  // put your main code here, to run repeatedly:
  delay(55000);
  getTemperature();

}

