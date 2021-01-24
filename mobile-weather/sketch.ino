#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11

const char* ssid = "";
const char* password = "";

uint8_t DHTPin = D8;

DHT dht(DHTPin, DHTTYPE);

float t;
float h;

void setup() {
  Serial.begin(9600);

  pinMode(DHTPin, INPUT);
  dht.begin(); 

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

void loop() {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      t = dht.readTemperature(); // Gets the values of the temperature
      h = dht.readHumidity(); // Gets the values of the humidity

      Serial.print("Temperature: ");
      Serial.println(t);

      Serial.print("Humidity: ");
      Serial.println(h);

      String serverPath = "http://192.168.1.95:5500/?a=5&temperature=";
      serverPath += t;
      serverPath += "&humidity=";
      serverPath += h;

      Serial.print(serverPath);
      
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    delay(60000);
}