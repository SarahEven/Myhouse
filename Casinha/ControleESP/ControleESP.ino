 
#include <FS.h>
#include <ArduinoJson.h>
#include <ws2812_i2s.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
#define NUM_LEDS 30 //quantidade de LEDs presentes na fita
 
const char* ssid = "nome da rede WiFi";
const char* password = "senha da rede WiFi";
 
String webPage = "";
 
int led = LED_BUILTIN;
 
static WS2812 ledstrip;
static Pixel_t pixels[NUM_LEDS];
 
ESP8266WebServer server(80);
 
// envia a página Web quando algum cliente conecta na raiz "/"
void handleRoot() {
  server.send(200, "text/html", webPage);
}
 
// recebe os dados RGB, imprime na serial e chama funcao para mudanca de cor da fita de LED
void handleColorData() {
  String data = server.arg("plain");
  server.send(204,"");
  StaticJsonBuffer<200> jBuffer;
  JsonObject& jObject = jBuffer.parseObject(data);
  int colorR = jObject["r"];
  int colorG = jObject["g"];
  int colorB = jObject["b"];
  Serial.print(colorR);
  Serial.print(" : ");
  Serial.print(colorG);
  Serial.print(" : ");
  Serial.println(colorB);
  changeColor(colorR, colorG, colorB);
  digitalWrite(led, !digitalRead(led));  
}
 
// muda a cor da fita de LED
void changeColor(int r, int g, int b) {
  int i;
  for(i=0;i<NUM_LEDS;i++) {
    pixels[i].R = r;
    pixels[i].G = g;
    pixels[i].B = b;
  }  
  ledstrip.show(pixels);
}
 
//Faz a leitura do arquivo HTML
void readFile(void) {
  File rFile = SPIFFS.open("/index.html","r");
  Serial.println("Lendo arquivo HTML...");
  webPage = rFile.readString();
  rFile.close();  
}
 
void setup() {
  pinMode(led, OUTPUT);
   
  Serial.begin(9600);
  SPIFFS.begin();
 
  if(SPIFFS.exists("/index.html"))
  {
    Serial.println("\n\nfile exists!");
  }
  else Serial.println("\n\nNo File :(");
 
  readFile();
 
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  if (MDNS.begin("iotlamp")) {
    Serial.println("MDNS responder started");
  }
 
  // aguarda por requisições na URL raiz "/" ou "/colordata"
  server.on("/", handleRoot);
  server.on("/colordata", handleColorData);
   
  server.begin();
 
  ledstrip.init(NUM_LEDS);
}
 
void loop() {
  server.handleClient();
}
