// #include <WiFi.h>
// #include <WebServer.h>
// #include <WebSocketsServer.h>
// #include "index.h"

// // #define LED_PIN 2

// #define ssid "TEST"
// #define password NULL

// WebServer server(80);
// WebSocketsServer webSocket = WebSocketsServer(81);

void handleRoot() { server.send(200, "text/html", htmlPage); }

void handleWebSocketMessage(uint8_t num, uint8_t *payload, size_t length) {
  String msg = "";
  for (size_t i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("[WebSocket ");
  Serial.print(num);
  Serial.print("] Received: ");
  Serial.println(msg);

  String response = "Response: " + msg;
  Serial.print("[WebSocket ");
  Serial.print(num);
  Serial.print("] Sending response: ");
  Serial.println(response);

  webSocket.sendTXT(num, response);
}

void onWebSocketEvent(uint8_t num, auto type, uint8_t *payload,
                      size_t length) {
  switch (type) {
  case WStype_CONNECTED: {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.print("[WebSocket ");
    Serial.print(num);
    Serial.print("] client connected: ");
    Serial.println(ip.toString());
    webSocket.sendTXT(num, "Connected to ESP32 WebSocket!");
    break;
  }

  case WStype_TEXT: {
    String msg = String((char *)payload);

    if (msg == "ping") {
      webSocket.sendTXT(num, "Response: pong");
    } else if (msg == "hello") {
      webSocket.sendTXT(num, "Response: world");
    } else {
      Serial.print("Received from the client ");
      Serial.print(num);
      Serial.print(": ");
      Serial.println(msg);

      webSocket.sendTXT(num, "Accepted: " + msg); // отправка обратно
    }
    break;
  }

  case WStype_DISCONNECTED: {
    Serial.print("[WebSocket ");
    Serial.print(num);
    Serial.println("] client disconnected");
    break;
  }
  }
}

// void onWebSocketClose(uint8_t num, auto type, uint8_t *payload,
//                       size_t length) {}

void WSS_setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("=== ESP32 WebSocket Server ===");
  Serial.print("Web page: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();

  webSocket.begin();
  webSocket.onEvent([](uint8_t num, auto type, uint8_t *payload,
                      size_t length) {
  switch (type) {
  case WStype_CONNECTED: {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.print("[WebSocket ");
    Serial.print(num);
    Serial.print("] client connected: ");
    Serial.println(ip.toString());
    webSocket.sendTXT(num, "Connected to ESP32 WebSocket!");
    break;
  }

  case WStype_TEXT: {
    String msg = String((char *)payload);

    if (msg == "ping") {
      webSocket.sendTXT(num, "Response: pong");
    } else if (msg == "hello") {
      webSocket.sendTXT(num, "Response: world");
    } else {
      Serial.print("Received from the client ");
      Serial.print(num);
      Serial.print(": ");
      Serial.println(msg);

      webSocket.sendTXT(num, "Accepted: " + msg); // отправка обратно
    }
    break;
  }

  case WStype_DISCONNECTED: {
    Serial.print("[WebSocket ");
    Serial.print(num);
    Serial.println("] client disconnected");
    break;
  }
  }
});
  Serial.print("WebSocket: ws://");
  Serial.print(WiFi.softAPIP());
  Serial.println(":81");
  Serial.print("Name of Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Password of Wi-Fi: ");
  Serial.println(password);

  Serial.println("Commands: ping, hello, any message");
}
