#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char ssid[] = "";
char pass[] = "";
int keyIndex = 0;

int status = WL_IDLE_STATUS;

struct StatusPage {
 String eta;
 String message;
 String orderStatus;
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {}
  connectToWiFi();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();

  String orderJson = getOrderStatus();
  Serial.println(orderJson);
  struct StatusPage deliveryStatus = getStatusObj(orderJson);

  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  //display.println(title);
  display.println("ETA: " + deliveryStatus.eta);
  display.println("");  
  display.println("Status: " + deliveryStatus.orderStatus);
  display.display();

 // display.setCursor(1,30);
 // display.println(doc["statusData"]["subtitle"]);  
}

StatusPage getStatusObj(String json) {
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return {"aa", "aa", "33"};
  }
  return { doc["eta"], doc["message"], doc["orderStatus"] };
}

void loop() {

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
