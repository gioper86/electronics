#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqttServer = "";

uint8_t rearPin1 = D3;
uint8_t rearPin2 = D2;
uint8_t rearEnable = D1;

uint8_t frontPin1 = D5;
uint8_t frontPin2 = D6;
uint8_t frontEnable = D8;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  
  pinMode(rearPin1, OUTPUT);
  pinMode(rearPin2, OUTPUT);
  pinMode(rearEnable, OUTPUT);
  pinMode(frontPin1, OUTPUT);
  pinMode(frontPin2, OUTPUT);
  pinMode(frontEnable, OUTPUT);

  setupWifi();

  client.setServer(mqttServer, 1883);
  client.setCallback(mqttCallback); 
}

void loop() {

  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();  
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
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("testGio");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void forward() {
  Serial.println("Forward");  
  digitalWrite(frontPin1, LOW);
  digitalWrite(frontPin2, HIGH);
  analogWrite(frontEnable, 700); //max 1023
}

void backward() {
  Serial.println("Backward");
  digitalWrite(frontPin1, HIGH);
  digitalWrite(frontPin2, LOW);
  analogWrite(frontEnable, 700);
}

void stopCar() {
  Serial.println("Stop");
  analogWrite(frontEnable, 0);
}

void straight() {
  Serial.println("Straight");   
  digitalWrite(rearEnable, LOW);  
}

void left() {
  Serial.println("Left");
  digitalWrite(rearPin1, LOW);
  digitalWrite(rearPin2, HIGH);
  digitalWrite(rearEnable, HIGH);  
}

void right() {
  Serial.println("Right");
  digitalWrite(rearPin1, HIGH);
  digitalWrite(rearPin2, LOW);
  digitalWrite(rearEnable, HIGH);  
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);

  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }
  Serial.println(messageTemp);

  if(messageTemp == "forward") {
    forward();  
  } else if(messageTemp == "backward") {
    backward(); 
  } else if(messageTemp == "stop") {
    stopCar();
  } else if(messageTemp == "left") {
    left();
  } else if(messageTemp == "right") {
    right();
  } else if(messageTemp == "straight") {
    straight();
  }    
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