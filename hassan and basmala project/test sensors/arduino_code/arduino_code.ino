#include <SPI.h>
#include <MFRC522.h>

// MFRC522 RFID reader pins
#define SS_PIN 10
#define RST_PIN 9

#define led_red 2
#define led_green 3
#define buzzer 5
#define fan 6

MFRC522 rfid(SS_PIN, RST_PIN);

// Relay pin
#define RELAY_PIN 4
#define RELAY_LED 7

// Correct RFID card UID (in uppercase, with spaces)
const String correctUID = "CE 9F FB 1D";

// Fan timing
unsigned long lastFanToggleTime = 0;
bool fanOn = false;
unsigned long lastScanTime = 0;  
bool startFanCycle = false;

// Bird detection flag
bool birdDetected = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();
  rfid.PCD_Init();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // relay off

  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(RELAY_LED, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(fan, LOW);

  Serial.println("System Ready...");
}

void loop() {
  checkRFID();   // handle RFID logic
  checkSerial(); // handle bird detection
  handleFan();   // handle fan timing
}

// ========== RFID ==========
void checkRFID() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String content = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      content.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(rfid.uid.uidByte[i], HEX));
      if (i < rfid.uid.size - 1) content.concat(" ");
    }
    content.toUpperCase();

    Serial.print("UID Detected: ");
    Serial.println(content);

    // Check UID
    if (content == correctUID) {
      Serial.println("Access Granted");
      digitalWrite(RELAY_PIN, LOW); // relay on
      digitalWrite(led_red, LOW);
      digitalWrite(led_green, HIGH);

      lastScanTime = millis();  // Start 1 min countdown for fan
      startFanCycle = true;
    } else {
      Serial.println("Invalid UID");
      digitalWrite(led_red, HIGH);
      digitalWrite(led_green, LOW);
    }

    delay(1000);
    digitalWrite(RELAY_PIN, HIGH); // relay off
    Serial.println("Ready for next scan...");
  }
}

// ========== FAN ==========
void handleFan() {
  if (startFanCycle) {
    unsigned long currentTime = millis();

    // After 1 minute, turn fan ON for 30 sec
    if (!fanOn && (currentTime - lastScanTime >= 60000)) {
      fanOn = true;
      lastFanToggleTime = currentTime;
      digitalWrite(fan, HIGH);
      Serial.println("Fan ON");
    }

    // Turn fan OFF after 30 sec
    if (fanOn && (currentTime - lastFanToggleTime >= 30000)) {
      fanOn = false;
      digitalWrite(fan, LOW);
      Serial.println("Fan OFF");
      // Reset 1-minute countdown
      lastScanTime = currentTime;
    }
  }
}

// ========== SERIAL ==========
void checkSerial() {
  if (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\n');

    if (receivedString.indexOf("B") != -1 || receivedString.indexOf("bird") != -1) {
      birdDetected = true;
    } else {
      birdDetected = false;
    }
  }

  if (birdDetected) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(led_red, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(led_red, LOW);
  }
}
