/*
Used Board ESP32 Dev Module
Usefull link:
https://github.com/OSSLibraries/Arduino_MFRC522v2
Connection:
RST  - 21
MISO - 19
MOSI - 23
SCK  - 18
SDA  - 5
*/

#include <SPI.h>
#include <MFRC522.h>

#define LedPin  2
#define SS_PIN  5
#define RST_PIN 21

// Inisialisasi Pin ESP32
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);    // Init serial communications
  SPI.begin();           // Init SPI bus
  rfid.PCD_Init();       // Init Modul RFID
  pinMode(LedPin, OUTPUT);  
  digitalWrite(LedPin, LOW);
  rfid.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  Serial.println("Scan kartu RFID untuk menampilkan UID.");
}

void loop() {
  // Periksa apakah ada kartu yang terdeteksi & Periksa apakah kartu dapat dibaca
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    Serial.print("UID tag :");
    String content= "";
    byte letter;
      for (byte i = 0; i < rfid.uid.size; i++) 
    {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();
    // Cek apakah UID yang terbaca adalah 73 91 B3 1A
    if (rfid.uid.size == 4 &&
        rfid.uid.uidByte[0] == 0x73 &&
        rfid.uid.uidByte[1] == 0x91 &&
        rfid.uid.uidByte[2] == 0xB3 &&
        rfid.uid.uidByte[3] == 0x1A) 
        {
        Serial.println("Kartu terdaftar terbaca! Menyalakan LED.");
        digitalWrite(LedPin, HIGH);
        delay(2500);
        digitalWrite(LedPin, LOW);
        }
      else {
        Serial.println("Kartu tidak dikenal.");
        digitalWrite(LedPin, LOW);
      } 
    // Berhenti komunikasi pada modul RFID
    rfid.PICC_HaltA(); 
  }
}