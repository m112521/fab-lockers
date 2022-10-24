#include <Wire.h>
#include <SPI.h>
#include <AmperkaFET.h>
#include <Adafruit_PN532.h>

#define PIN_CS  A0
#define PN532_IRQ   9

FET mosfet(PIN_CS);
Adafruit_PN532 nfc(PN532_IRQ, 100); // 100 - any number higher than pins number

uint8_t uid0[] = {};
uint8_t uid1[] = {};
uint8_t uid2[] = {};
uint8_t uid3[] = {};
uint8_t uid4[] = {};

boolean compareUid(uint8_t uidRead[8], uint8_t uidComp[8], uint8_t uidLen) {
  for (uint8_t i = 0; i < uidLen; i++) {
    if (uidRead[i] != uidComp[i]) {
      return false;
    }
    if (i == (uidLen)-0x01) {
      return true;
    }
  }
}

void toggleLock(int uid) {
  mosfet.digitalWrite(uid, HIGH);
  delay(2000);

  mosfet.digitalWrite(ALL, LOW);
}

void setup() {
  mosfet.begin();

  Serial.begin(9600);
  nfc.begin();
  int versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find RFID/NFC reader");
    while(1) {
    }
  }
 
  Serial.println("Found RFID/NFC reader");
  nfc.SAMConfig();
  Serial.println("Waiting for a card ...");
}
 
void loop() {
  uint8_t success;
  uint8_t uid[8]; // ID buffer
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    if (compareUid(uid, uid0, uidLength)) {
      toggleLock(0);
      //Serial.println("user 0");
    } 
    else if (compareUid(uid, uid1, uidLength)) {
      toggleLock(1);
    }
    else if (compareUid(uid, uid2, uidLength)) {
      toggleLock(2);
    }
    else if (compareUid(uid, uid3, uidLength)) {
      toggleLock(3);
    }
    else if (compareUid(uid, uid4, uidLength)) {
      toggleLock(4);
    }
    else {
      Serial.println("not found");
    }
    delay(1000);
  }
}

