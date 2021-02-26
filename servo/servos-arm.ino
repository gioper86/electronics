#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqttServer = "";

WiFiClient espClient;
PubSubClient client(espClient);
 
Servo servo;
Servo servo2;

int leftRightAngle = 90;
int bottomTopAngle = 90;
int STEP = 15;
 
void setup() {
    Serial.begin(9600);
    setupWifi();

    client.setServer(mqttServer, 1883);
    client.setCallback(mqttCallback);    
    
    servo.attach(5); //D1
    servo.write(leftRightAngle);

    servo2.attach(14); //D5
    servo2.write(bottomTopAngle);    
}

void loop() {
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);

  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }
  Serial.println(messageTemp);

  if(messageTemp == "right") {
    right();
  } else if(messageTemp == "left") {
    left();
  } else if(messageTemp == "top") {
    top();
  } else if(messageTemp == "bottom") {
    bottom();
  }

  Serial.println((String)"leftRightAngle: " + leftRightAngle);
  Serial.println((String)"bottomTopAngle: " + bottomTopAngle);
}

void right() {
  leftRightAngle = max(leftRightAngle - STEP, 0);
  servo.write(leftRightAngle);
}

void left() {
  leftRightAngle = min(leftRightAngle + STEP, 180);
  servo.write(leftRightAngle);
}

void top() {
  bottomTopAngle = min(bottomTopAngle + STEP, 180);
  servo2.write(bottomTopAngle);
}

void bottom() {
  bottomTopAngle = max(bottomTopAngle - STEP, 0);
  servo2.write(bottomTopAngle);
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("servoCamera");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
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