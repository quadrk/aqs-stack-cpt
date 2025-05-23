#ifndef CONFIG_H
#define CONFIG_H

#include <EEPROM.h>

// Структура конфигурации
struct Configuration {
  char wifi_ssid[32];
  char wifi_password[64];
  char api_server[128];
  char api_key[128];
  uint8_t is_configured; // Флаг, чтобы проверить, была ли конфигурация уже настроена
};

// Адрес в EEPROM для хранения конфигурации
#define EEPROM_CONFIG_ADDRESS 0
#define CONFIG_SIGNATURE 0xAF

// Функции для работы с конфигурацией
void loadConfig();
void saveConfig();
void clearConfig();
bool isConfigured();

// Getters
const char* getWifiSSID();
const char* getWifiPassword();
const char* getApiServer();
const char* getApiKey();

// Setters
void setWifiSSID(const char* ssid);
void setWifiPassword(const char* password);
void setApiServer(const char* server);
void setApiKey(const char* key);

#endif
