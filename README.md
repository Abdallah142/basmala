# 🌱 Eco FarmTech & RFID Fan Control Systems

This repository contains two IoT and embedded systems projects:

- **Eco FarmTech** – A Smart Farm Monitoring System using ESP32, which monitors temperature, humidity, soil moisture, gas, flame, pH, and water levels.  
  It provides a WiFi web dashboard with pump control and displays live data on a 20x4 I2C LCD.

- **RFID Fan & Bird Detection System** – An Arduino-based security and automation system using RFID authentication, a fan timer, and a bird detection alert (with buzzer, LEDs, and relay).

---

## 📌 Project 1: Eco FarmTech (ESP32 Smart Farm Monitoring)

### 🔧 Features
- 📶 **ESP32 Access Point** – Creates its own WiFi network for local dashboard access.  
- 🌡️ **DHT11 Sensor** – Reads temperature and humidity.  
- 🌱 **Capacitive Soil Moisture Sensor** – Reads soil humidity level.  
- 🔥 **Gas Sensor (MQ2)** – Detects harmful gases.  
- 🔥 **Flame Sensor** – Detects fire hazards.  
- 💧 **pH Sensor** – Monitors soil water pH level.  
- 🏞️ **Water Level Sensor** – Indicates tank status (LOW/FULL).  
- 💡 **20x4 I2C LCD Display** – Real-time values.  
- 💦 **Pump Control** – Turn irrigation pump ON/OFF via web dashboard buttons.  

---

### 🖥️ Web Dashboard
- Shows all sensor readings.  
- Displays status indicators (**Safe**, **Alert**, **Not Connected**).  
- Includes **Pump ON/OFF buttons**.  
- Mobile-friendly with **Font Awesome icons**.  

---

### 🛠️ Hardware Used
- ESP32  
- DHT11 sensor  
- Capacitive soil moisture sensor  
- MQ2 gas sensor  
- Flame sensor  
- Analog pH sensor  
- Digital water level sensor  
- Relay + water pump  
- 20x4 I2C LCD  

---

### 📸 System Dashboard Example
*(Insert screenshot of your web dashboard here)*

---

## 📌 Project 2: RFID Fan & Bird Detection (Arduino)

### 🔧 Features
- 🔑 **RFID Authentication** – Only a specific card/tag can activate the system.  
- 🚪 **Relay Control** – Unlocks/activates connected devices (e.g., door or motor).  
- 🌬️ **Fan Automation** –  
  - After valid RFID scan, waits **1 min**, then runs the fan for **30s**.  
  - Repeats cycle until a new scan.  
- 🐦 **Bird Detection (via Serial Input)** –  
  - If "bird" is detected, buzzer and red LED alert.  
- 🔴🟢 **LED Indicators** – Access status and alerts.  
- 🔊 **Buzzer** – Audible warning for bird detection.  

---

### 🛠️ Hardware Used
- Arduino Uno / Nano  
- MFRC522 RFID reader module  
- Relay module  
- DC fan  
- LEDs (Red & Green)  
- Buzzer  
- Power supply  

---

## 🚀 How to Use

### 🔹 Eco FarmTech (ESP32)
1. Upload the code: **EcoFarmTech.ino**  
2. Connect to WiFi AP → **SSID:** `Eco FarmTech` / **Password:** `12345678`  
3. Open browser → `http://192.168.4.1`  
4. View live dashboard & control pump.  

### 🔹 RFID Fan System (Arduino)
1. Upload the code: **RFID_FanSystem.ino**  
2. Open **Serial Monitor** at `9600 baud`.  
3. Scan an RFID card.  
   - If valid UID → Relay activates & fan cycle starts.  
   - If "bird" is detected via serial → Buzzer & red LED alert.  

---

## 📂 Repository Structure

---

## 📸 Hardware Setup
*(Add wiring diagrams or Fritzing schematics here)*  

---

## 🧑‍💻 hassan && basmala
👤 Developed by **[abdallah]**  
📧 Contact: **your.email@example.com**  

---

## 📜 License
This project is licensed under the **MIT License** – free to use, modify, and distribute.
