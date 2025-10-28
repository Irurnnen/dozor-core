void RFID_setup() {
  SPI.begin();
  mfrc522.PCD_Init();
}

String getUID() {
    if (!mfrc522.PICC_IsNewCardPresent()) {  // нет новой карты в поле
        delay(5);
        return "no_new_card";
    }
    if (!mfrc522.PICC_ReadCardSerial()) {  // ошибка чтения UID
        delay(5);
        return "read_error";
    }

    // записываем UID
    String UID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (i) UID += ":";
        if (mfrc522.uid.uidByte[i] < 0x10) UID += "0";
        UID += String(mfrc522.uid.uidByte[i], HEX);
    }
    UID.toUpperCase();

    ledFlash(0, 60, 60, 60, 40, 1);  // бирюзовая вспышка — чтение
    // printEventJson(mfrc522.uid);              // печать события
    ledFlash(0, 60, 0, 120, 60, 2);  // зелёные вспышки — успех

    mfrc522.PICC_HaltA();  // завершение сессии с картой
    mfrc522.PCD_StopCrypto1();

    // «антизалипание» считывания при удержании карты: краткая пауза
    unsigned long t0 = millis();
    while (millis() - t0 < 300) {
        if (!mfrc522.PICC_IsNewCardPresent()) break;
        delay(30);
    }
    ledColor(0, 0, 60, 20);  // возврат в режим ожидания

    return UID;
}
