#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h>   // 🔹 for software reset

// RFID pins
#define SS_PIN 10
#define RST_PIN 9

// LEDs & Relay pins
#define led_red 2
#define led_green 3
#define RELAY_PIN 8

// RFID reader object
MFRC522 rfid(SS_PIN, RST_PIN);

// Correct UID
const String correctUID = "CE 9F FB 1D";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Start locked (relay HIGH = locked if Active LOW relay)
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(RELAY_PIN, HIGH);

  Serial.println("Place your RFID card...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Read UID
  String UID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    UID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    UID += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) UID += " ";
  }
  UID.toUpperCase();

  Serial.print("Card UID: ");
  Serial.println(UID);

  if (UID == correctUID) {
    Serial.println("Access Granted ✅");
    digitalWrite(led_green, HIGH);
    digitalWrite(RELAY_PIN, LOW);   // Unlock
    delay(2000);                    // Keep unlocked 2 sec
    digitalWrite(led_green, LOW);
    digitalWrite(RELAY_PIN, HIGH);  // Lock again

    // 🔄 Auto-reset Arduino after success
    Serial.println("Restarting Arduino...");
    wdt_enable(WDTO_15MS);  // watchdog reset in ~15ms
    while (1) {}            // wait for reset
  } else {
    Serial.println("Access Denied ❌");
    digitalWrite(led_red, HIGH);
    delay(2000);
    digitalWrite(led_red, LOW);
  }

  // release card so it can be read again
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
