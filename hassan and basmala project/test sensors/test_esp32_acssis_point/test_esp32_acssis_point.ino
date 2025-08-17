#include <WiFi.h>
#include <WebServer.h>

// SSID and password for Access Point
const char* ssid = "Eco FarmTech";
const char* password = "12345678";  // minimum 8 chars

// Create web server on port 80
WebServer server(80);

// HTML content
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Eco FarmTech</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { text-align: center; font-family: Arial; margin-top: 50px; }
    h1 { color: green; }
  </style>
</head>
<body>
  <h1>Welcome to Eco FarmTech 🌱</h1>
  <p>Your smart eco farming system is ready.</p>
  
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(115200);

  // Start Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}
