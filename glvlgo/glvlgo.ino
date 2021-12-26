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

char status1[] = "{\"orderUpdateTime\":1636718715001,\"page\":\"STATUS\",\"secondsToNextRequest\":30,\"statusData\":{\"title\":\"Tempo stimato di arrivo\",\"subtitle\":\"13:30-13:40\",\"animationUrl\":\"https://res.cloudinary.com/glovoapp/raw/upload/v1579251834/CX/ongoing_order/animations/food_processing.json\",\"body\":\"Il ristorante ha ricevuto il tuo glovo!\",\"progressData\":{\"steps\":[{\"weight\":37,\"status\":\"ANIMATED\",\"label\":\"Preparazione\"},{\"weight\":26,\"status\":\"EMPTY\",\"label\":\"Ritiro\"},{\"weight\":37,\"status\":\"EMPTY\",\"label\":\"Consegna\"}]},\"mainContent\":{\"type\":\"ANIMATION\",\"animationData\":{\"url\":\"someurl\"}},\"etaNotice\":null,\"etaLowerBound\":null,\"etaUpperBound\":null,\"oldEtaUpperBound\":null},\"lastIncidentData\":null}";
char status2[] = "{\"orderUpdateTime\":1636719461836,\"page\":\"STATUS\",\"secondsToNextRequest\":30,\"statusData\":{\"title\":\"Tempo stimato di arrivo\",\"subtitle\":\"13:30-13:40\",\"animationUrl\":\"https:\/\/res.cloudinary.com\/glovoapp\/raw\/upload\/v1579251835\/CX\/ongoing_order\/animations\/food_pickup.json\",\"body\":\"Il corriere sta <b>ritirando<\/b> il tuo ordine.\",\"progressData\":{\"steps\":[{\"weight\":37,\"status\":\"FILLED\",\"label\":\"Preparazione\"},{\"weight\":26,\"status\":\"ANIMATED\",\"label\":\"Ritiro\"},{\"weight\":37,\"status\":\"EMPTY\",\"label\":\"Consegna\"}]},\"mainContent\":{\"type\":\"ANIMATION\",\"animationData\":{\"url\":\"https:\/\/res.cloudinary.com\/glovoapp\/raw\/upload\/v1579251835\/CX\/ongoing_order\/animations\/food_pickup.json\"}},\"etaNotice\":null,\"etaLowerBound\":null,\"etaUpperBound\":null,\"oldEtaUpperBound\":null},\"lastIncidentData\":null}";
char status3[] = "{\"orderUpdateTime\":1636720083004,\"page\":\"MAP\",\"secondsToNextRequest\":10,\"mapData\":{\"courierData\":{\"location\":{\"latitude\":41.38675437778686,\"longitude\":2.1672309778523586},\"icons\":{\"leftIconUrl\":\"v1576079370\/CX\/ongoing_order\/transport\/moto\/left\/fill\",\"rightIconUrl\":\"v1576079370\/CX\/ongoing_order\/transport\/moto\/right\/fill\"},\"message\":\"Arrivo l\u00EC in un batter d'occhio!\"},\"stateDescriptionData\":{\"title\":\"Ancora 14 minuti\",\"subtitle\":null,\"animationUrl\":\"https:\/\/res.cloudinary.com\/glovoapp\/raw\/upload\/v1579251835\/CX\/ongoing_order\/animations\/general_moto-glover.json\",\"body\":\"Il tuo ordine \u00E8 in fase di <b>consegna<\/b>!\",\"progressData\":{\"steps\":[{\"weight\":37,\"status\":\"FILLED\",\"label\":null},{\"weight\":26,\"status\":\"FILLED\",\"label\":null},{\"weight\":37,\"status\":\"ANIMATED\",\"label\":null}]},\"mainContent\":{\"type\":\"ANIMATION\",\"animationData\":{\"url\":\"https:\/\/res.cloudinary.com\/glovoapp\/raw\/upload\/v1579251835\/CX\/ongoing_order\/animations\/general_moto-glover.json\"}},\"etaNotice\":null,\"etaLowerBound\":null,\"etaUpperBound\":null,\"oldEtaUpperBound\":null},\"points\":[{\"type\":\"PICKUP\",\"title\":\"Ronda de la Universitat, 31, Barcelona, Spain, Bacoa\",\"details\":\"Bacoa\",\"iconUrl\":\"v1576079374\/CX\/ongoing_order\/pin\/regular-empty\",\"coordinate\":{\"latitude\":41.387157,\"longitude\":2.167171}},{\"type\":\"DELIVERY\",\"title\":\"Carrer de Pallars, 190, oficina Glovo\",\"details\":\"oficina Glovo\",\"iconUrl\":\"v1576079374\/CX\/ongoing_order\/flag\/active\",\"coordinate\":{\"latitude\":41.4009881,\"longitude\":2.1978804}}]}}";

// char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {}
  Serial.println("Connected to wifi");
  // connectToWiFi();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, status3);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char* page = doc["page"];
  Serial.println(page);

  // String title = String();
  String title = doc["statusData"]["title"];
  String subTitle = doc["statusData"]["subtitle"];
  String body = doc["statusData"]["body"];

  struct StatusPage stat1 = {title, subTitle, "Preparation"};  

  getStatusObj(status2);

  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  //display.println(title);
  display.println("ETA: " + subTitle);
  display.println("");  
  display.println(body);
  display.display();

 // display.setCursor(1,30);
 // display.println(doc["statusData"]["subtitle"]);  
}

StatusPage getStatusObj(char* json) {
  
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return {"aa", "aa", "33"};
  }
  const char* page = doc["page"];
  Serial.println(page);
  struct StatusPage deliveryStatus;

  if (page == "STATUS") {
    
  }

  String title = doc["statusData"]["title"];
  String subTitle = doc["statusData"]["subtitle"];
  String body = doc["statusData"]["body"];

  struct StatusPage stat2 = {subTitle, body, "Preparation"};  

  Serial.println("stat1" + stat2.eta);
  return stat2;
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
    delay(10000);
  }
}

void getOrderStatus() {
  char serverAddress[] = "127.0.0.1";
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, serverAddress, 5500);

  client.get("/glvorder");
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
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
