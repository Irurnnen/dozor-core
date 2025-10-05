#include <MFRC522.h>
MFRC522 rfid(10, 9); // SDA, RST

void RFID_setup() {
  Serial.begin(115200);
  SPI.begin();      // запуск шины
  rfid.PCD_Init();  // инициализация модуля
}

void RFID_loop() {
  // если поднесена метка
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // запишем метку в 4 байта
    uint32_t ID;
    for (byte i = 0; i < 4; i++) {
      ID <<= 8;
      ID |= rfid.uid.uidByte[i];
    }

    // выведем
    Serial.println(ID, HEX);
    delay(500);
  }
}