void loop() {
  unsigned long now = millis();

  
  // String data = "AA:BB:CC:DD:EE:FF";
  // webSocket.broadcastTXT(data);
  // delay(2000);
  static unsigned long lastWS = 0;
  if (now - lastWS >= 2000) {
    lastWS = now;
    String data = getUID();
    // String data = "AA:BB:CC:DD:EE:FF";
    if (data == "no_new_card") return;
    if (data == "read_error"){
      webSocket.broadcastTXT("Card reading error");
      Serial.println("Card reading error");
      return;
    }

    data = printEventJson(data);

    Serial.println(data);
    webSocket.broadcastTXT(data);
  }

  handleSerialCommands();

  webSocket.loop();
  server.handleClient();
  delay(1);
}
