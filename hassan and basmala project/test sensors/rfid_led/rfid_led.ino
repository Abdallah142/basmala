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

// Correct RFID card UID (in uppercase, with spaces)
const String correctUID = "CE 9F FB 1D";

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

  digitalWrite(buzzer, LOW);

  digitalWrite(led_red, LOW);

  digitalWrite(led_green, LOW);

  Serial.println("RFID Scanner Ready...");
}

void loop() {
  // Look for new card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Read UID
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
  } else {
    Serial.println("Invalid UID");
    digitalWrite(led_red, HIGH);
    digitalWrite(led_green, LOW);
  }

  delay(3000);
  digitalWrite(RELAY_PIN, HIGH); // relay off
  Serial.println("Ready for next scan...");
}
