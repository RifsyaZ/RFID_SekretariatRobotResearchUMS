#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <rdm6300.h>
#include "data.h"

#define RDM6300_RX_PIN 16
#define RST_PIN 27
#define SS_PIN  21
#define relay   2
#define indi    5

MFRC522 mfrc522(SS_PIN, RST_PIN);
Rdm6300 rdm6300;

String content, content2;

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(indi, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  rdm6300.begin(RDM6300_RX_PIN);
  Serial.println("GO");
  digitalWrite(indi, HIGH);
}

void loop() {
  if (rdm6300.get_new_tag_id()) {
    Serial.print("ID:");
    content2 = "";
    content2.concat(String(rdm6300.get_tag_id(), HEX));
    Serial.println(content2);

    for (int a = 0; a <= 168; a++) {
      if (content2 == id[a]) {
        digitalWrite(indi, LOW);
        digitalWrite(relay, HIGH);
        delay(2000);
        ESP.restart();
      }
    }
  }

  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print("ID:");
      content = "";
      for (int i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println(content);
      for (int a = 0; a <= 168; a++) {
        if (content == id[a]) {
          digitalWrite(indi, LOW);
          digitalWrite(relay, HIGH);
          delay(2000);
          ESP.restart();
        }
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
  }
}
