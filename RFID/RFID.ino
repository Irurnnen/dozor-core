/*
 * ESP32-S3 + RC522 (SPI) + DS3231 (I2C) + встроенный NeoPixel
 * Вывод: одна строка JSON на каждое считывание карты/брелка.
 * Формат при наличии RTC:
 *   {"uid":"FB:F0:B5:02","type":"MIFARE 1KB","ts":"2025-10-02 00:52:59","ts_src":"rtc"}
 * Формат при отсутствии RTC:
 *   {"uid":"FB:F0:B5:02","type":"MIFARE 1KB","ts_src":"no_rtc"}
 *
 * Установка времени RTC по Serial:
 *   T=2025-10-02 12:34:56
 */

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
/* ---------- JSON ---------- */
#include <ArduinoJson.h>

#include "index.h"

#define ssid "DOZOR"                 // имя wifi
#define password "liQXsLNAJ8nd9V4l"  // пароль от wifi

#define Server_port 80               // порт, на котором открывается сервер
#define WSS_port 81                  // порт, на котором открывается web socket

WebServer server(Server_port);
WebSocketsServer webSocket = WebSocketsServer(WSS_port);

/* ---------- ПИНЫ ---------- */
// RC522 (SPI)
#define RC522_SS 10    // SS (SDA на модуле RC522)
#define RC522_RST 7    // RST
#define RC522_SCK 12   // SCK
#define RC522_MISO 13  // MISO
#define RC522_MOSI 11  // MOSI
// DS3231 (I2C)
#define I2C_SDA 8  // SDA
#define I2C_SCL 9  // SCL
// Встроенный NeoPixel (часто GPIO48 на ESP32-S3 DevKit)
#define LED_PIN 48
#define LED_COUNT 1
/* ------------------------------------------------------- */

MFRC522 mfrc522(RC522_SS, RC522_RST);  // Экземпляр RC522
RTC_DS3231 rtc;                        // Экземпляр DS3231
Adafruit_NeoPixel pixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

static bool rtc_ok = false;  // Флаг доступности DS3231