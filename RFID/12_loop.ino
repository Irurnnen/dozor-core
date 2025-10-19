void loop() {
  unsigned long now = millis();

  // String data = printEventJson(getUID);
  // String data = "AA:BB:CC:DD:EE:FF";
  // webSocket.broadcastTXT(data);
  // delay(2000);
  static unsigned long lastWS = 0;
  if (now - lastWS >= 2000) {
    lastWS = now;
    String data = "AA:BB:CC:DD:EE:FF";
    webSocket.broadcastTXT(data);
  }

  webSocket.loop();
  server.handleClient();
  delay(1);
}
