// #include <WiFi.h>
// #include <DNSServer.h>
// // #include <SSD1306Wire.h>

// // Access Point credentials
// const char *ssid = "TEST";
// const char *password = NULL;
// int connections = 0;

// // Onboard WiFi server
// WiFiServer server(80);
// String responseHTML = "<!DOCTYPE html><html>"
//                       "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
//                       "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
//                       "</style></head>"
//                       "<body><h1>ESP32 Web Server</h1>"
//                       "<p>Hello World</p>"
//                       "</body></html>";

// OLED Display 128x64
// SSD1306Wire  display(0x3c, 5, 4);

// void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
//   connections += 1;
//   showConnectionsCount();
// }

// void showConnectionsCount() {
//   char data[32];
//   sprintf(data, "Connections: %d", connections);
//   draw_message(data);
// }

void WiFi_setup() {                  
  // Serial.println();
  // Serial.println("Configuring access point...");

  // Start access point 
  WiFi.mode(WIFI_AP);                   
  WiFi.softAP(ssid, password);
  // WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_AP_STACONNECTED); //я так понял это нужно для отслеживания кол-ва подключенных

  IPAddress ip_address = WiFi.softAPIP();     //IP Address of our accesspoint

  // Start web server
  server.begin();
  
  Serial.print("AP IP address: ");
  Serial.println(ip_address);

  // // Oled display 
  // display.init();
  // // Draw info
  // display.clear();
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.setFont(ArialMT_Plain_10);
  // display.drawString(0, 0, "Access Point started");
  // display.drawString(0, 12, ssid);
  // display.drawString(0, 24, "AP IP address: ");
  // display.drawString(0, 36, ip_address.toString());
  // display.display();

  // Total number of connections
  // showConnectionsCount();
}

void draw_message(char *msg) {
  // display.setColor(BLACK);
  // display.fillRect(0, 50, display.getWidth(), 12);
  // display.setColor(WHITE);
  // display.drawString(0, 50, msg);
  // display.display();

  Serial.println(msg);
}

void WiFi_loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    draw_message("Client connected");
    
    String currentLine = "";   // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {        // if there's bytes to read from the client,
        char c = client.read();      // read a byte, then
        Serial.write(c);                // print it out the serial monitor
        if (c == '\n') {                 // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // Send header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println(responseHTML);            
            // The HTTP response ends with another blank line
            client.println();
            break;
          } else { // if we got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if we got anything else but a CR character,
          currentLine += c;   // add it to the end of the currentLine
        }
      }
    }
    // Close the connection
    client.stop();
    // showConnectionsCount();
  }
}