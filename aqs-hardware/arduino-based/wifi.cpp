#include "wifi.h"
#include "config.h"
#include "rtc.h"
#include <WiFiS3.h>
#include <Arduino_LED_Matrix.h>
#include <RTC.h>

ArduinoLEDMatrix matrix;

const uint32_t fullOn[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
const uint32_t fullOff[] = {0x00000000, 0x00000000, 0x00000000};

void connectWiFiInternal(const char* ssid, const char* password) {
    // Проверяем, настроены ли значения WiFi
    if (strlen(ssid) == 0 || strlen(password) == 0) {
        Serial.println("WiFi credentials not configured. Please use 'wifi <SSID> <PASSWORD>' command.");
        return;
    }

    matrix.begin();
    WiFi.begin(ssid, password);
    Serial.print("Connecting Wi-Fi to ");
    Serial.print(ssid);
    Serial.print(" ");

    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
        matrix.loadFrame(fullOn); delay(250);
        matrix.loadFrame(fullOff); delay(250);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi connected");
        unsigned long ipAttempt = millis();
        while ((WiFi.localIP() == INADDR_NONE || WiFi.localIP() == IPAddress(0,0,0,0))
                && millis() - ipAttempt < 5000) {
            delay(100);
        }

        IPAddress ip = WiFi.localIP();
        if (ip != INADDR_NONE && ip != IPAddress(0,0,0,0)) {
            Serial.print("IP: "); Serial.println(ip);
            matrix.loadFrame(fullOn);

            if (RTC.begin()) {
                syncTimeWithNTP();
            } else {
                Serial.println("RTC initialization failed");
            }
        } else {
            Serial.println("Failed to obtain IP!");
            matrix.loadFrame(fullOff);
        }
    } else {
        Serial.println("\nWi-Fi connection failed");
        matrix.loadFrame(fullOff);
    }
}

void connectWiFi() {
    // Используем значения из конфигурации
    connectWiFiInternal(getWifiSSID(), getWifiPassword());
}

void reconnectWiFi(const char* ssid, const char* password) {
    WiFi.disconnect();
    delay(500);

    // Обновляем конфигурацию
    setWifiSSID(ssid);
    setWifiPassword(password);

    // Сохраняем изменения в EEPROM
    saveConfig();

    // Подключаемся с новыми настройками
    connectWiFiInternal(getWifiSSID(), getWifiPassword());
}

String getCurrentTimeISO8601() {
    RTCTime currentTime;
    if (RTC.getTime(currentTime)) {
        char isoTime[30];
        sprintf(isoTime, "%04d-%02d-%02dT%02d:%02d:%02dZ",
            currentTime.getYear(),
            (int)currentTime.getMonth() + 1,  // +1: месяц начинается с 0
            currentTime.getDayOfMonth(),
            currentTime.getHour(),
            currentTime.getMinutes(),
            currentTime.getSeconds()
        );
        return String(isoTime);
    } else {
        Serial.println("Failed to obtain RTC time");
        return "1970-01-01T00:00:00Z";
    }
}

