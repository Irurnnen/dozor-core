/* ----------- Печать JSON ----------- */
static String printEventJson(const String uid) {
  String juid = "{\"uid\":\"" + uid;
  // Serial.print("{\"uid\":\"");
  // Serial.print(uid);
   
  // Serial.print("\",\"type\":\"");
  MFRC522::PICC_Type t = mfrc522.PICC_GetType(mfrc522.uid.sak);
  // Serial.print((__FlashStringHelper*)mfrc522.PICC_GetTypeName(t));
  juid += "\",\"type\":\"" + String (mfrc522.PICC_GetTypeName(t));

  // Serial.print("\"");
  juid += "\"";

  if (rtc_ok) {
    DateTime now = rtc.now();
    String year, month, day, hour, minute, second;
    year = now.year();
    month = now.month();
    day = now.day();
    hour = now.hour();
    minute = now.minute();
    second = now.second();

    juid += ",\"ts\":\"" + year + "-";
    juid += month + "-";
    juid += day + " ";
    juid += hour + ":";
    juid += minute + ":" + second;
    // Serial.print(",\"ts\":\"");
    // Serial.printf("%04d-%02d-%02d %02d:%02d:%02d",
    //               now.year(), now.month(), now.day(),
    //               now.hour(), now.minute(), now.second());

    juid += "\",\"ts_src\":\"rtc\"}\n";
    // Serial.print("\",\"ts_src\":\"rtc\"}\n");
  } else {
    // RTC отсутствует/недоступен — время не выводим
    Serial.print(",\"ts_src\":\"no_rtc\"}\n");
    juid += ",\"ts_src\":\"no_rtc\"}\n";
  }
  Serial.print(juid);
  Serial.flush();
  return juid;
}


void setup(){
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  
  RFID_setup();
  WSS_setup();
}
