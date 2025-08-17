#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// ====== WiFi Access Point ======
const char* ssid = "Eco FarmTech";
const char* password = "12345678"; 

WebServer server(80);

// ====== Sensors ======
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_PIN 34
#define MQ2_PIN 35
#define FLAME_PIN 25     // 🔄 Changed from 32 → 25 (to free GPIO32 for water level)
#define PUMP_PIN 2
#define PH_PIN 33        // 🔄 Changed to GPIO33
#define WATER_PIN 32     // ✅ Digital water level sensor

// ====== LCD ======
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ====== Sensor Values ======
float t, h;
int soil, gas, flame, ph;
bool pumpState = false;
int waterLevel = LOW;

// ====== HTML Page ======
String htmlPage() {
  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Eco FarmTech</title>
    <meta charset="UTF-8">
    <style>
      body { font-family: Arial, sans-serif; background: #f2f2f2; text-align: center; }
      h1 { color: #2e8b57; }
      .sensor {
        background: white; padding: 15px; margin: 10px auto; width: 300px;
        border-radius: 12px; box-shadow: 0 4px 8px rgba(0,0,0,0.2);
        font-size: 18px;
      }
      .ok { color: green; }
      .alert { color: red; font-weight: bold; }
      i { margin-right: 8px; }
      .btn {
        display: inline-block; margin: 10px; padding: 10px 20px;
        border: none; border-radius: 8px; font-size: 16px; cursor: pointer;
        color: white;
      }
      .on { background: green; }
      .off { background: red; }
    </style>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
  </head>
  <body>
    <h1><i class="fa-solid fa-leaf"></i> Eco FarmTech Dashboard</h1>
    <div class="sensor"><i class="fa-solid fa-temperature-half"></i> Temperature: )rawliteral";
    
  if (isnan(t)) page += "Not Connect";
  else page += String(t) + " °C";

  page += R"rawliteral(</div>
    <div class="sensor"><i class="fa-solid fa-droplet"></i> Humidity: )rawliteral";
  if (isnan(h)) page += "Not Connect";
  else page += String(h) + " %";

  page += R"rawliteral(</div>
    <div class="sensor"><i class="fa-solid fa-water"></i> Soil Moisture: )rawliteral";
  if (soil < 1) page += "Not Connect";
  else page += String(soil);

  page += R"rawliteral(</div>
    <div class="sensor"><i class="fa-solid fa-fire"></i> Gas: )rawliteral";
  page += (gas == HIGH) ? "<span class='alert'>Detected!</span>" : "<span class='ok'>Safe</span>";

  page += R"rawliteral(</div>
    <div class="sensor"><i class="fa-solid fa-fire-extinguisher"></i> Flame: )rawliteral";
  page += (flame == HIGH) ? "<span class='alert'>FIRE!</span>" : "<span class='ok'>Safe</span>";

  page += R"rawliteral(</div>
    <div class="sensor"><i class="fa-solid fa-flask"></i> pH: )rawliteral";
  if (ph < 1) page += "Not Connect";
  else page += String(ph);

  page += R"rawliteral(</div>
    <div class="sensor"><i class="fa-solid fa-tint"></i> Water Level: )rawliteral";
  if (waterLevel == HIGH) page += "<span class='ok'>FULL</span>";
  else if (waterLevel == LOW) page += "<span class='alert'>LOW</span>";
  else page += "Not Connect";

  page += R"rawliteral(</div>

    <h2>Pump Control</h2>
    <form action="/pumpOn" method="GET">
      <button class="btn on"><i class="fa-solid fa-play"></i> Pump ON</button>
    </form>
    <form action="/pumpOff" method="GET">
      <button class="btn off"><i class="fa-solid fa-stop"></i> Pump OFF</button>
    </form>

    <p>Current Pump State: )rawliteral";
  page += (pumpState) ? "<span class='ok'>ON</span>" : "<span class='alert'>OFF</span>";
  page += R"rawliteral(</p>

  </body>
  </html>)rawliteral";

  return page;
}

// ====== Update Sensors ======
void updateSensors() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  soil = analogRead(SOIL_PIN);
  gas = digitalRead(MQ2_PIN);
  flame = digitalRead(FLAME_PIN);
  ph = analogRead(PH_PIN);
  waterLevel = digitalRead(WATER_PIN);
}

// ====== Update LCD ======
void updateLCD() {
  lcd.clear();
  
  lcd.setCursor(0,0);
  if (isnan(t) || isnan(h)) lcd.print("DHT11: Not Connect");
  else lcd.print("T:" + String(t) + "C H:" + String(h) + "%");

  lcd.setCursor(0,1);
  if (soil < 1) lcd.print("Soil: Not Connect");
  else lcd.print("Soil:" + String(soil));

  lcd.setCursor(0,2);
  if (gas == HIGH) lcd.print("Gas: Detected!");
  else lcd.print("Gas: Safe");

  lcd.setCursor(0,3);
  if (waterLevel == HIGH) lcd.print("Water: FULL");
  else lcd.print("Water: LOW");
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  dht.begin();
  
  lcd.begin();
  lcd.backlight();
  
  pinMode(FLAME_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(WATER_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);

  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started: " + String(ssid));

  // صفحة الداتا
  server.on("/", [](){
    updateSensors();
    server.send(200, "text/html", htmlPage());
  });

  // تشغيل المضخة
  server.on("/pumpOn", [](){
    digitalWrite(PUMP_PIN, LOW);
    pumpState = true;
    server.sendHeader("Location","/");
    server.send(303);
  });

  // إيقاف المضخة
  server.on("/pumpOff", [](){
    digitalWrite(PUMP_PIN, HIGH);
    pumpState = false;
    server.sendHeader("Location","/");
    server.send(303);
  });

  server.begin();
}

// ====== Loop ======
void loop() {
  server.handleClient();
  updateSensors();
  updateLCD();
  delay(2000);
}
