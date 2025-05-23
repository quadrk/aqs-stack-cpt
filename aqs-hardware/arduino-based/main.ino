#include "wifi.h"
#include "config.h"
#include "sender.h"
#include "command.h"
#include "sensors.h"
#include <EEPROM.h>

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 60000; // отправка каждые 60 секунд

void setup() {
  Serial.begin(115200);

  // Загрузка конфигурации из EEPROM
  loadConfig();

  // Инициализация датчиков
  for (int i = 0; i < sensorCount; i++) {
    sensors[i]->init();
  }

  // Попытка подключения к WiFi, если есть конфигурация
  if (strlen(getWifiSSID()) > 0 && strlen(getWifiPassword()) > 0) {
    connectWiFi();
  }

  Serial.println("\nAir Quality Monitor - Arduino R4 WiFi + SCD30");
  Serial.println("Available commands:");
  Serial.println("  wifi <SSID> <PASSWORD> - Set WiFi credentials");
  Serial.println("  apiserver <url> - Set API server URL");
  Serial.println("  apikey <key> - Set API key");
  Serial.println("  read - Read sensor data");
  Serial.println("  send - Send measurements to server");
  Serial.println("  config - Show current configuration");
  Serial.println("  clear - Clear all configuration");
  Serial.println("  reboot - Reboot device");
}

void loop() {
  processSerial();

  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= sendInterval) {
    // Проверяем, настроены ли WiFi и API
    if (isConfigured() && WiFi.status() == WL_CONNECTED &&
        strlen(getApiServer()) > 0 && strlen(getApiKey()) > 0) {

      String now = getCurrentTimeISO8601();
      if (!now.startsWith("1970")) {
        Serial.println("Auto-send triggered");
        sendAllMeasurements(sensors, sensorCount, wifi);
      } else {
        Serial.println("RTC time invalid, skipping auto-send");
      }
    } else {
      if (!isConfigured()) {
        Serial.println("Device not configured, skipping auto-send");
      } else if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected, skipping auto-send");
      } else {
        Serial.println("API configuration missing, skipping auto-send");
      }
    }

    lastSendTime = currentTime;
  }

  delay(100);
}
