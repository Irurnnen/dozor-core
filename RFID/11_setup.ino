/* ------------------- УТИЛИТЫ LED ------------------- */
static inline void ledColor(uint8_t r, uint8_t g, uint8_t b, uint8_t br = 20) {
    pixel.setBrightness(br);
    pixel.setPixelColor(0, pixel.Color(r, g, b));
    pixel.show();
}
static inline void ledFlash(uint8_t r, uint8_t g, uint8_t b,
                            int on_ms = 120, int off_ms = 80, int times = 2) {
    for (int i = 0; i < times; i++) {
        ledColor(r, g, b, 40);
        delay(on_ms);
        ledColor(0, 0, 0, 0);
        delay(off_ms);
    }
}


/* ----------- Парсер команд установки времени RTC ----------- */
/* Формат команды:  T=YYYY-MM-DD HH:MM:SS  (закончить \n) */
static void handleSerialCommands() {
    while (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        line.trim();
        if (!line.startsWith("T=")) continue;

        int yy, MM, dd, hh, mm, ss;
        if (sscanf(line.c_str() + 2, "%d-%d-%d %d:%d:%d",
                &yy, &MM, &dd, &hh, &mm, &ss)== 6) {
        if (rtc_ok) {
            rtc.adjust(DateTime(yy, MM, dd, hh, mm, ss));
            Serial.println("{\"set_time\":\"ok\"}");
            ledFlash(0, 60, 0, 100, 60, 2);  // зелёный — время принято
        } else {
            Serial.println("{\"set_time\":\"no_rtc\"}");
            ledFlash(60, 0, 0, 120, 80, 2);  // красный — RTC недоступен
        }
        } else {
        Serial.println("{\"set_time\":\"bad_format\",\"hint\":\"T=2025-10-02 12:34:56\"}");
        ledFlash(60, 40, 0, 100, 60, 1);  // жёлтый — неверный формат
        }
    }
}


/* ----------- Печать JSON ----------- */
static String makeEventJson(String uid) {
    StaticJsonDocument<200> doc;

    doc["uid"] = uid;

    MFRC522::PICC_Type t = mfrc522.PICC_GetType(mfrc522.uid.sak);
    doc["type"] = mfrc522.PICC_GetTypeName(t);

    if (rtc_ok){ // если модуль времени работает
        DataTime now = rtc.now();
        doc["ts"] = now.unixtime();
        doc["ts_src"] = "rtc";
    } else { // если модуль времени не работает
        doc["ts_src"] = "no_rtc";
    }

    String output;
    serializeJson(doc, output);
    return output
}


void setup() {
    Serial.begin(115200);
    delay(200);

    RFID_setup();
    WSS_setup();

    pixel.begin();
    ledColor(60, 0, 60, 40);  // фиолетовый — инициализация

    Wire.begin(I2C_SDA, I2C_SCL);  // I2C на заданных пинах
    Wire.setClock(100000);         // 100 кГц
    if (rtc.begin()) {
        rtc_ok = true;
        if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // первичная инициализация
        Serial.println("RTC: time set from compile time (lost power).");
        ledFlash(60, 40, 0, 400, 200, 1);  // жёлтый — восстановлено после сброса
        }
    } else {
        rtc_ok = false;
        Serial.println("RTC: not found (I2C).");
        ledColor(60, 0, 0, 40);  // красный — RTC недоступен
        delay(600);
    }

    SPI.begin(RC522_SCK, RC522_MISO, RC522_MOSI, RC522_SS);  // SPI на заданных пинах
    mfrc522.PCD_Init();                                      // Инициализация RC522
    mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);          // Макс. усиление антенны

    Serial.println("Ready. Tap a card...");
    ledColor(0, 0, 60, 20);  // синий — режим ожидания
}
