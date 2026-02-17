#include <WiFi.h>
#include "wificonfig.h"


const char* ssid = WIFI_SSID; 
const char* password = WIFI_PASSWORD;

#define LIGHTPIN 26
#define FANPIN 25
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
WiFiServer server(80);
bool fan = false;
bool light = true;


void setup();
void pressButton(uint8_t pin);
void loop();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  server.begin();
  Serial.println(WiFi.localIP());
  Serial.println("Server started");

  pinMode(LIGHTPIN, OUTPUT);
  digitalWrite(LIGHTPIN, HIGH);  
  pinMode(FANPIN, OUTPUT);
  digitalWrite(FANPIN, HIGH);    
}

void pressButton(uint8_t pin) {
  digitalWrite(pin, LOW);  
  delay(150);                
  digitalWrite(pin, HIGH);   
  Serial.print("button ");
  Serial.print(pin);
  Serial.println(" hit");
} 


void loop() {
  WiFiClient client = server.available();

  if (client)
  {
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";
    String header = "";
    bool handler = false;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') { 
          if (header.indexOf("\r\n\r\n") != -1 && !handler) { 
          handler = true;
          if (header.indexOf("GET /LIGHTS/toggle") >= 0) {
            light = true;
            Serial.println("ACTIVATED");
            client.println("HTTP/1.1 204 No Content");
            client.println("Connection: close");
            client.println();
            break;
          }
          
          if (header.indexOf("GET /FAN/toggle") >= 0) {
              fan = true;
              client.println("HTTP/1.1 204 No Content");
              client.println("Connection: close");
              client.println();
              break;
          }

          // main page serving
          if (header.indexOf("GET / ") >= 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>");
            client.println("html { font-family: Helvetica; text-align: center; }");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 { background-color: #555555; }");
            client.println("</style></head>");
            client.println("<script>");
            client.println("function toggleLight() { fetch('/LIGHTS/toggle', { method : 'GET', mode: 'no-cors'}); }");
            client.println("function toggleFan() { fetch('/FAN/toggle', { method : 'GET', mode: 'no-cors'}); }");
            client.println("</script>");
            client.println("<body><h1>my room lights hir</h1>");
            client.println("<p>LIGHTS</p>");
            client.println("<p><button class =\"button\" onclick=\"toggleLight()\">TOGGLE</button>");
            client.println("<p>FAN</p>");
            client.println("<p><button class =\"button\" onclick=\"toggleFan()\">TOGGLE</button>");
            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r')
          currentLine += c;
        }
      }
    }
    header = "";
    //----handle button press----
    //occurs outside of while loop to allow handshake to complete
    //TODO: switch order of client.stop and light handler. HTTP request should close before pin flash
    if (light)
    {
      pressButton(LIGHTPIN); 
      light = false;
    }
    if (fan)
    { 
      pressButton(FANPIN);
      fan = false;
    }
  //close handshake
  client.stop();
  Serial.println("Disconnected");
  }
}


