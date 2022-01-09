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
bool pollOrders = true;

int status = WL_IDLE_STATUS;
int xPosition = 0;

struct StatusPage {
 String eta;
 String message;
 String orderStatus;
};

class LcdAnimation {
  int xPosition;
  int screenWidth;
  int imageWidth;
  unsigned long millisMarker;
  
  public:
  LcdAnimation(int sw, int imageWidth) {
    xPosition = -imageWidth;
    screenWidth = sw;
    millisMarker = 0;
  }

  bool animate() {
    if (!isFinished() && (millis() - millisMarker) > 100) {
      xPosition = xPosition + 2;
      millisMarker = millis();
      // refreshDisplay(deliveryStatus, bmp);
      return true;
    }
    return false;
  }

  int getXPosition() {
    return xPosition;
  }

  bool isFinished() {
    if(xPosition >= screenWidth) {
      xPosition = -imageWidth;
      return true;
    }
    return false;
  }
};

// 'pizza', 35x34px
unsigned char epd_bitmap_pizza [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 
  0x63, 0xe0, 0x00, 0x00, 0x00, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x70, 0x3c, 0x00, 0x00, 0x00, 0x7e, 
  0x1e, 0x00, 0x00, 0x00, 0x7f, 0x0f, 0x00, 0x00, 0x00, 0x7f, 0xc3, 0x80, 0x00, 0x00, 0x63, 0xe1, 
  0xc0, 0x00, 0x00, 0xe0, 0xf0, 0xe0, 0x00, 0x00, 0xc0, 0x78, 0x70, 0x00, 0x01, 0xc0, 0x3c, 0x38, 
  0x00, 0x01, 0x9f, 0x9e, 0x38, 0x00, 0x03, 0xbf, 0xcf, 0x1c, 0x00, 0x03, 0x39, 0xc7, 0x8e, 0x00, 
  0x07, 0x30, 0xc3, 0x8e, 0x00, 0x06, 0x30, 0xc1, 0xc7, 0x00, 0x0e, 0x39, 0xc1, 0xc3, 0x00, 0x0e, 
  0x1f, 0xc0, 0xe3, 0x80, 0x1c, 0x0f, 0x9e, 0xf1, 0x80, 0x1c, 0x00, 0x3f, 0x71, 0x80, 0x18, 0x00, 
  0x3f, 0x71, 0xc0, 0x39, 0xf0, 0x33, 0x38, 0xc0, 0x33, 0xf0, 0x3f, 0x38, 0xc0, 0x73, 0x30, 0x1f, 
  0x38, 0xc0, 0x63, 0xf0, 0x00, 0xfc, 0xe0, 0xe1, 0xf0, 0x07, 0xff, 0xc0, 0xc0, 0xe0, 0x7f, 0x8f, 
  0xc0, 0xc0, 0x07, 0xf8, 0x02, 0x00, 0xc0, 0xff, 0x80, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x00, 
  0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

struct StatusPage deliveryStatus;
LcdAnimation lcdAnimation(SCREEN_WIDTH, 35);

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 560)
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, 10);
  display.println("Connecting...");
  display.display();

  while (!Serial) {}
  //connectToWiFi();
}


void loop() {
  if(pollOrders) {
    //String orderJson = getOrderStatus();
    //Serial.println(orderJson);
    deliveryStatus = {"14 minuti", "", "Delivering"}; //getStatusObj(orderJson);
  }

  while(!lcdAnimation.isFinished()) {
    if(lcdAnimation.animate()) {
      displayOrderStatus(deliveryStatus, lcdAnimation.getXPosition());
    }
  }

  delay(1000);
}

void displayOrderStatus(StatusPage deliveryStatus, int xPosition) {
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  //display.println(title);
  display.println("ETA: " + deliveryStatus.eta);
  display.println("");  
  display.println("Status: " + deliveryStatus.orderStatus);

  display.drawBitmap(xPosition, 30, epd_bitmap_pizza, 35, 45, WHITE);
  
  display.display(); 
}

StatusPage getStatusObj(String json) {
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
