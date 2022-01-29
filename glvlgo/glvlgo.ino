#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include "LcdDisplay.cpp"
#include "LcdAnimation.cpp"
#include "LegoMan.cpp"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

char ssid[] = "";
char pass[] = "";
int keyIndex = 0;
bool pollOrders = true;

int status = WL_IDLE_STATUS;
int xPosition = 0;


LcdDisplay lcdDisplay;
LcdAnimation lcdAnimation(SCREEN_WIDTH, 35);
LegoMan legoMan(3, 150, 180);

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 560)
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcdDisplay.initDisplay();

  while (!Serial) {}
  //connectToWiFi();
}

void loop() {
  DeliveryStatus deliveryStatus;
  if(pollOrders) {
    //String orderJson = getOrderStatus();
    //Serial.println(orderJson);
    deliveryStatus = {"14 minuti", "", "Delivering"}; //getStatusObj(orderJson);
  }

  legoMan.setDeliveryStatus(deliveryStatus);

  while(!lcdAnimation.isFinished() || !legoMan.isFinished()) {
    if(lcdAnimation.animate()) {
      // TODO try to move to LcdAnimation class
      lcdDisplay.displayOrderStatus(deliveryStatus, lcdAnimation.getXPosition());
    }
    legoMan.animate();
  }
    
  delay(1000);
  legoMan.reset();
  lcdAnimation.reset();
}

DeliveryStatus getStatusObj(String json) {
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return {"", "", "Error"};
  }
  return { doc["eta"], doc["message"], doc["orderStatus"] };
}

void connectToWiFi() {
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true)
      ;
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    Serial.print("status" + status);
    delay(5000);
  }
}

String getOrderStatus() {
  char serverAddress[] = "";
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, serverAddress, 5500);

  client.get("/glvorder");
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  return response;
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
