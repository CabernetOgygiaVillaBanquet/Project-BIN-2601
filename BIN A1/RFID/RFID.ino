#include "SPI.h"
#include "MFRC522.h"

#define RST_PIN  9 // RES pin
#define SS_PIN  10 // SDA (SS) pin

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
   Serial.begin(9600);
   SPI.begin();
   mfrc522.PCD_Init();
   delay(4);
   mfrc522.PCD_DumpVersionToSerial();
   Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
   // réinitialiser le cycle s'il n'y a pas de carte sur le lecteur
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
   }

   if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
   }

   // affiche l'UID sur le port série
   mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
