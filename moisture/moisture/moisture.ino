#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqttServer = "";

const int analogInPin = A0;
const int powerPin = D2;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(powerPin, OUTPUT);
  
  Serial.begin(9600);
  //TODO: go to sleep if it can't connect to WiFi or MQTT broker
  WiFi.mode(WIFI_STA);
  setupWifi();
  client.setServer(mqttServer, 1883);
  mqttReconnect();
 
  while(!Serial) { }

  //getReadings();
}

void loop() {
  digitalWrite(powerPin, HIGH);
  for(int i=0;i<10;i++) {
    int moistureReading = analogRead(analogInPin);
    Serial.println(moistureReading);
    String message = (String)moistureReading;
    client.publish("moisture", message.c_str());    
    delay(500);
  }
  digitalWrite(powerPin, LOW);
  client.disconnect();
  Serial.println("Going to deep sleep for 600 seconds");
  ESP.deepSleep(600e6);  
}

float* getReadings() {}

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
