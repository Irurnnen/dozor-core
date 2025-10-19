#include <SPI.h>
#include <Wire.h>
/* ---------- WiFi ---------- */
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
/* ---------- UID СЧИТЫВАТЕЛЬ ---------- */
#include <MFRC522.h>
/* ---------- ВРЕМЯ ---------- */
#include <RTClib.h>
/* ---------- СВЕТОДИОДЫ ---------- */
#include <Adafruit_NeoPixel.h>

#include "index.h"

#define ssid "TEST"
#define password NULL

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

/* ---------- ПИНЫ ---------- */
// RC522 (SPI)
#define RC522_SS 5     // SS (SDA на модуле RC522)
#define RC522_RST 4    // RST
#define RC522_SCK 18   // SCK
#define RC522_MISO 19  // MISO
#define RC522_MOSI 23  // MOSI
// DS3231 (I2C)
#define I2C_SDA 21  // SDA
#define I2C_SCL 22  // SCL
// Встроенный NeoPixel (часто GPIO48 на ESP32-S3 DevKit)
#define LED_PIN 48
#define LED_COUNT 1
/* ------------------------------------------------------- */

MFRC522 mfrc522(RC522_SS, RC522_RST);  // Экземпляр RC522
RTC_DS3231 rtc;                        // Экземпляр DS3231
Adafruit_NeoPixel pixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

static bool rtc_ok = false;  // Флаг доступности DS3231