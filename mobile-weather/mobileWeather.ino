#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11

float t;
float h;

const char* ssid = "";
const char* password = "";
const char* mqttServer = "";

const int DHTPin = D7;
const int analogInPin = A0;
int batteryValue = 0;

WiFiClient espClient;
DHT dht(DHTPin, DHTTYPE);
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  setupWifi();
  client.setServer(mqttServer, 1883);
  mqttReconnect();
 
  pinMode(DHTPin, INPUT);
  dht.begin(); 
  while(!Serial) { }

  getReadings();
  
  client.disconnect();
  Serial.println("Going to deep sleep for 600 seconds");
  ESP.deepSleep(600e6);
}

void loop() { }

float* getReadings() {

  float lastT;
  float lastH;

  //need to get several readings from the sensor before it gives accurate values
  for(int i=0;i<10;i++) {
    batteryValue = analogRead(analogInPin);
    t = dht.readTemperature();
    h = dht.readHumidity();

    lastT = t;
    lastH = h;

    delay(1200);
  }
  
  batteryValue = analogRead(analogInPin);
  String message = (String)lastT + "," + (String)lastH + "," + (String)batteryValue;
  client.publish("mobileTemp", message.c_str());
}

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());  
}


void mqttReconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      // client.subscribe("testGio");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}