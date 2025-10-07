void RFID_setup() {
  SPI.begin();
  mfrc522.PCD_Init();
}

void RFID_loop() {
  // Проверяем, есть ли новая карта
  // if (!mfrc522.PICC_IsNewCardPresent()) return;

  // Пробуем считать UID
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Останавливаем работу с картой
  mfrc522.PICC_HaltA();
}
