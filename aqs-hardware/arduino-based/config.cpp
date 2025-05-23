#include "config.h"
#include <Arduino.h>
#include <EEPROM.h>

// Глобальная конфигурация
static Configuration config;

void loadConfig() {
  // Считывание конфигурации из EEPROM (побайтно для совместимости с Arduino R4)
  for (int i = 0; i < sizeof(Configuration); i++) {
    *((byte*)&config + i) = EEPROM.read(EEPROM_CONFIG_ADDRESS + i);
  }

  // Проверка, была ли конфигурация инициализирована
  if (config.is_configured != CONFIG_SIGNATURE) {
    // Устанавливаем дефолтные пустые значения
    memset(&config, 0, sizeof(Configuration));
    config.wifi_ssid[0] = '\0';
    config.wifi_password[0] = '\0';
    config.api_server[0] = '\0';
    config.api_key[0] = '\0';
    config.is_configured = 0; // Не сконфигурировано

    Serial.println("No valid configuration found. Using defaults (empty).");
  } else {
    Serial.println("Configuration loaded from EEPROM.");
  }
}

void saveConfig() {
  // Устанавливаем флаг, что конфигурация настроена
  config.is_configured = CONFIG_SIGNATURE;

  // Сохраняем конфигурацию в EEPROM (побайтно для совместимости с Arduino R4)
  for (int i = 0; i < sizeof(Configuration); i++) {
    EEPROM.update(EEPROM_CONFIG_ADDRESS + i, *((byte*)&config + i));
  }
  Serial.println("Configuration saved to EEPROM.");
}

void clearConfig() {
  // Очищаем конфигурацию
  memset(&config, 0, sizeof(Configuration));
  config.is_configured = 0;

  // Сохраняем пустую конфигурацию в EEPROM (побайтно для совместимости с Arduino R4)
  for (int i = 0; i < sizeof(Configuration); i++) {
    EEPROM.update(EEPROM_CONFIG_ADDRESS + i, *((byte*)&config + i));
  }
  Serial.println("Configuration cleared.");
}

bool isConfigured() {
  return config.is_configured == CONFIG_SIGNATURE;
}

// Getters
const char* getWifiSSID() {
  return config.wifi_ssid;
}

const char* getWifiPassword() {
  return config.wifi_password;
}

const char* getApiServer() {
  return config.api_server;
}

const char* getApiKey() {
  return config.api_key;
}

// Setters
void setWifiSSID(const char* ssid) {
  strncpy(config.wifi_ssid, ssid, sizeof(config.wifi_ssid) - 1);
  config.wifi_ssid[sizeof(config.wifi_ssid) - 1] = '\0'; // Ensure null-termination
}

void setWifiPassword(const char* password) {
  strncpy(config.wifi_password, password, sizeof(config.wifi_password) - 1);
  config.wifi_password[sizeof(config.wifi_password) - 1] = '\0';
}

void setApiServer(const char* server) {
  strncpy(config.api_server, server, sizeof(config.api_server) - 1);
  config.api_server[sizeof(config.api_server) - 1] = '\0';
}

void setApiKey(const char* key) {
  strncpy(config.api_key, key, sizeof(config.api_key) - 1);
  config.api_key[sizeof(config.api_key) - 1] = '\0';
}
