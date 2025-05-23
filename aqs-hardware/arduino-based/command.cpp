#include "command.h"
#include "wifi.h"
#include "config.h"
#include "sender.h"
#include "Sensor.h"

#include <Arduino.h>
#include <string.h>

extern Sensor* sensors[];
extern const int sensorCount;
extern WiFiClient wifi;

static char inputBuffer[128];
static int inputPos = 0;

void handleCommand(char* command) {
  char* token = strtok(command, " ");

  if (token && strcmp(token, "wifi") == 0) {
    char* ssid = strtok(NULL, " ");
    char* password = strtok(NULL, " ");
    if (ssid && password) {
      reconnectWiFi(ssid, password);
    } else {
      Serial.println("Invalid command format. Use: wifi <SSID> <PASSWORD>");
    }
  } else if (token && strcmp(token, "apiserver") == 0) {
    char* server = strtok(NULL, " ");
    if (server) {
      setApiServer(server);
      saveConfig();
      Serial.print("API server updated to: "); Serial.println(getApiServer());
    } else {
      Serial.println("Invalid command format. Use: apiserver <url>");
    }
  } else if (token && strcmp(token, "apikey") == 0) {
    char* key = strtok(NULL, " ");
    if (key) {
      setApiKey(key);
      saveConfig();
      Serial.print("API key updated to: "); Serial.println(getApiKey());
    } else {
      Serial.println("Invalid command format. Use: apikey <key>");
    }
  } else if (token && strcmp(token, "read") == 0) {
    printSensorData(sensors, sensorCount);
  } else if (token && strcmp(token, "send") == 0) {
    sendAllMeasurements(sensors, sensorCount, wifi);
  } else if (token && strcmp(token, "config") == 0) {
    // Вывод текущей конфигурации
    Serial.println("\nCurrent configuration:");
    Serial.print("WiFi SSID: "); Serial.println(getWifiSSID());
    Serial.print("WiFi Password: "); Serial.println(getWifiPassword());
    Serial.print("API Server: "); Serial.println(getApiServer());
    Serial.print("API Key: "); Serial.println(getApiKey());
    Serial.print("Is configured: "); Serial.println(isConfigured() ? "Yes" : "No");
  } else if (token && strcmp(token, "clear") == 0) {
    // Очистка всей конфигурации
    clearConfig();
    Serial.println("Configuration cleared. Device will need to be reconfigured.");
  } else if (token && strcmp(token, "reboot") == 0) {
    Serial.println("Rebooting...");
    delay(500);
    NVIC_SystemReset();
  } else {
    Serial.println("Unknown command. Available commands:");
    Serial.println("  wifi <SSID> <PASSWORD> - Set WiFi credentials");
    Serial.println("  apiserver <url> - Set API server URL");
    Serial.println("  apikey <key> - Set API key");
    Serial.println("  read - Read sensor data");
    Serial.println("  send - Send measurements to server");
    Serial.println("  config - Show current configuration");
    Serial.println("  clear - Clear all configuration");
    Serial.println("  reboot - Reboot device");
  }
}

void processSerial() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputPos > 0) {
        inputBuffer[inputPos] = '\0';
        handleCommand(inputBuffer);
        inputPos = 0;
      }
    } else if (inputPos < sizeof(inputBuffer) - 1) {
      inputBuffer[inputPos++] = c;
    }
  }
}