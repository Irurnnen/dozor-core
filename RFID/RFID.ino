#include <WiFi.h>
#include <DNSServer.h>

// Access Point credentials
#define ssid "TEST"  // name of Wi-Fi
#define password NULL // password of Wi-Fi
// int connections = 0;

// Onboard WiFi server
WiFiServer server(80);
String responseHTML = "<!DOCTYPE html><html>"
                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
                      "</style></head>"
                      "<body><h1>esp32 Web Server</h1>"
                      "<p>Hello World</p>"
                      "</body></html>";

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
// VCC -> 3.3
// RST -> 9
// GND -> GND
// MISO -> 13
// MOSI -> 11
// SCK -> 12
// SDA -> 10
