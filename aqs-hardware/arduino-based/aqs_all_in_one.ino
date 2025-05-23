/*
 * AQS - Air Quality System
 * 
 * Этот файл содержит полную реализацию системы мониторинга качества воздуха
 * на базе Arduino R4 WiFi и датчика SCD30 от Seeed Studio.
 * 
 * Система измеряет:
 * - Концентрацию CO2 (ppm)
 * - Температуру (°C)
 * - Относительную влажность (%)
 * 
 * Данные отправляются на сервер через WiFi и могут быть просмотрены через API.
 */

// ========== БИБЛИОТЕКИ ==========
#include <EEPROM.h>           // Для хранения конфигурации
#include <WiFiS3.h>           // WiFi для Arduino R4
#include <Wire.h>             // I2C коммуникация
#include <SCD30.h>      // Библиотека для датчика SCD30
#include <RTC.h>              // Часы реального времени
#include <WiFiUdp.h>          // UDP для NTP
#include <NTPClient.h>        // Синхронизация времени
#include <Arduino_LED_Matrix.h> // LED-матрица на Arduino R4
#include <ArduinoHttpClient.h>  // HTTP клиент
#include <ArduinoJson.h>      // Работа с JSON

// ========== КОНСТАНТЫ ==========
#define EEPROM_CONFIG_ADDRESS 0    // Адрес для хранения конфигурации в EEPROM
#define CONFIG_SIGNATURE 0xAF      // Сигнатура для проверки инициализации конфигурации

// Интервал отправки данных (60 секунд = 1 минута)
const unsigned long SEND_INTERVAL = 60000;

// Константы для идентификации данных с датчика
const char* SENSOR_LABELS[] = {"CO2", "T", "RH"};
const int SENSOR_DATA_COUNT = 3; // CO2, температура, влажность

// Анимация для LED-матрицы
const uint32_t LED_FULL_ON[] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
const uint32_t LED_FULL_OFF[] = {0x00000000, 0x00000000, 0x00000000};

// ========== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ==========
// Структура конфигурации для хранения в EEPROM
struct Configuration {
  char wifi_ssid[32];        // SSID WiFi сети
  char wifi_password[64];    // Пароль WiFi сети
  char api_server[128];      // URL сервера API
  char api_key[128];         // Ключ API
  uint8_t is_configured;     // Флаг инициализации
};

// Глобальные экземпляры объектов
WiFiClient wifiClient;        // WiFi клиент
ArduinoLEDMatrix matrix;      // LED-матрица
// Экземпляр SCD30 scd30 уже объявлен в библиотеке и доступен через extern
WiFiUDP ntpUDP;               // UDP для NTP
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000); // NTP клиент

// Глобальная конфигурация
Configuration config;

// Переменные для отслеживания времени
unsigned long lastSendTime = 0;  // Время последней отправки данных

// Буфер для обработки команд
char inputBuffer[128];
int inputPos = 0;

// ========== ФУНКЦИИ КОНФИГУРАЦИИ ==========

/**
 * Загружает конфигурацию из EEPROM
 */
void loadConfig() {
  // Считывание конфигурации из EEPROM (побайтно)
  for (int i = 0; i < sizeof(Configuration); i++) {
    *((byte*)&config + i) = EEPROM.read(EEPROM_CONFIG_ADDRESS + i);
  }

  // Проверка сигнатуры
  if (config.is_configured != CONFIG_SIGNATURE) {
    // Инициализация пустой конфигурации
    memset(&config, 0, sizeof(Configuration));
    config.is_configured = 0;
    config.wifi_ssid[0] = '\0';
    config.wifi_password[0] = '\0';
    config.api_server[0] = '\0';
    config.api_key[0] = '\0';
    
    Serial.println("No valid configuration found. Using defaults (empty).");
  } else {
    Serial.println("Configuration loaded from EEPROM.");
  }
}

/**
 * Сохраняет конфигурацию в EEPROM
 */
void saveConfig() {
  // Устанавливаем флаг, что конфигурация настроена
  config.is_configured = CONFIG_SIGNATURE;

  // Сохраняем в EEPROM
  for (int i = 0; i < sizeof(Configuration); i++) {
    EEPROM.update(EEPROM_CONFIG_ADDRESS + i, *((byte*)&config + i));
  }
  Serial.println("Configuration saved to EEPROM.");
}

/**
 * Очищает всю конфигурацию
 */
void clearConfig() {
  // Очищаем конфигурацию
  memset(&config, 0, sizeof(Configuration));
  config.is_configured = 0;

  // Сохраняем пустую конфигурацию в EEPROM
  for (int i = 0; i < sizeof(Configuration); i++) {
    EEPROM.update(EEPROM_CONFIG_ADDRESS + i, *((byte*)&config + i));
  }
  Serial.println("Configuration cleared.");
}

/**
 * Проверяет, инициализирована ли конфигурация
 */
bool isConfigured() {
  return config.is_configured == CONFIG_SIGNATURE;
}

// Getters для конфигурации
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

// Setters для конфигурации
void setWifiSSID(const char* ssid) {
  strncpy(config.wifi_ssid, ssid, sizeof(config.wifi_ssid) - 1);
  config.wifi_ssid[sizeof(config.wifi_ssid) - 1] = '\0';
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

// ========== ФУНКЦИИ WIFI И NTP ==========

/**
 * Синхронизирует время через NTP
 */
void syncTimeWithNTP() {
  Serial.println("Starting NTP sync...");
  timeClient.begin();

  int attempts = 0;
  const int maxAttempts = 10;

  while (attempts < maxAttempts) {
    if (timeClient.forceUpdate()) {
      unsigned long epoch = timeClient.getEpochTime();
      if (epoch > 1000000000UL && epoch < 2000000000UL) {
        Serial.print("NTP time: "); Serial.println(epoch);
        RTCTime rtcTime(epoch);
        RTC.setTime(rtcTime);
        Serial.println("RTC time updated from NTP.");
        return;
      } else {
        Serial.print("Invalid epoch received: "); Serial.println(epoch);
      }
    }

    Serial.print("NTP sync attempt "); Serial.print(attempts + 1); Serial.println(" failed.");
    attempts++;
    delay(2000);
  }

  Serial.println("Failed to sync time via NTP after multiple attempts.");
}

/**
 * Получает текущее время в формате ISO8601
 */
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

/**
 * Подключение к WiFi сети с использованием настроек из конфигурации
 */
void connectWiFi() {
  // Проверяем наличие креденшиалов
  if (strlen(config.wifi_ssid) == 0 || strlen(config.wifi_password) == 0) {
    Serial.println("WiFi credentials not configured. Use 'wifi <SSID> <PASSWORD>' command.");
    return;
  }

  // Инициализация LED-матрицы
  matrix.begin();
  
  // Начинаем подключение к WiFi
  WiFi.begin(config.wifi_ssid, config.wifi_password);
  Serial.print("Connecting Wi-Fi to ");
  Serial.print(config.wifi_ssid);
  Serial.print(" ");

  // Ожидаем подключения, отображая прогресс на LED-матрице
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    matrix.loadFrame(LED_FULL_ON); delay(250);
    matrix.loadFrame(LED_FULL_OFF); delay(250);
    Serial.print(".");
  }

  // Проверяем результат подключения
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected");
    
    // Ожидаем получения IP-адреса
    unsigned long ipAttempt = millis();
    while ((WiFi.localIP() == INADDR_NONE || WiFi.localIP() == IPAddress(0,0,0,0))
            && millis() - ipAttempt < 5000) {
      delay(100);
    }

    // Отображаем IP-адрес
    IPAddress ip = WiFi.localIP();
    if (ip != INADDR_NONE && ip != IPAddress(0,0,0,0)) {
      Serial.print("IP: "); Serial.println(ip);
      matrix.loadFrame(LED_FULL_ON);  // Индикация успешного подключения

      // Инициализируем RTC и синхронизируем время
      if (RTC.begin()) {
        syncTimeWithNTP();
      } else {
        Serial.println("RTC initialization failed");
      }
    } else {
      Serial.println("Failed to obtain IP!");
      matrix.loadFrame(LED_FULL_OFF);
    }
  } else {
    Serial.println("\nWi-Fi connection failed");
    matrix.loadFrame(LED_FULL_OFF);
  }
}

/**
 * Переподключение к WiFi с новыми параметрами
 */
void reconnectWiFi(const char* ssid, const char* password) {
  // Отключаемся от текущей сети
  WiFi.disconnect();
  delay(500);

  // Обновляем конфигурацию
  setWifiSSID(ssid);
  setWifiPassword(password);

  // Сохраняем конфигурацию
  saveConfig();

  // Подключаемся с новыми параметрами
  connectWiFi();
}

// ========== ФУНКЦИИ ДЛЯ РАБОТЫ С SCD30 ==========

/**
 * Инициализация датчика SCD30
 */
void initSensor() {
  // Инициализация датчика
  scd30.initialize();
  
  Serial.println("SCD30 sensor initialized");
  delay(1000); // Небольшая задержка для завершения инициализации
}

/**
 * Чтение данных с датчика SCD30
 * 
 * @param data      Буфер для хранения считанных данных
 * @param dataSize  Размер буфера
 * @return true, если данные успешно считаны, false в противном случае
 */
bool readSensorData(float* data, int dataSize) {
  // Проверяем, достаточно ли места в буфере
  if (dataSize < SENSOR_DATA_COUNT) return false;

  // Проверяем доступность данных
  if (scd30.isAvailable()) {
    float result[SENSOR_DATA_COUNT];
    scd30.getCarbonDioxideConcentration(result);
    
    // Копируем результаты в буфер
    for (int i = 0; i < SENSOR_DATA_COUNT; i++) {
      data[i] = result[i];
    }
    return true;
  }
  
  // Данные недоступны
  return false;
}

/**
 * Вывод данных с датчика в Serial
 */
void printSensorData() {
  float dataBuffer[SENSOR_DATA_COUNT];
  if (readSensorData(dataBuffer, SENSOR_DATA_COUNT)) {
    for (int j = 0; j < SENSOR_DATA_COUNT; j++) {
      Serial.print(SENSOR_LABELS[j]);
      Serial.print(": ");
      Serial.println(dataBuffer[j]);
    }
  } else {
    Serial.println("No data available");
  }
}

// ========== ФУНКЦИИ ОТПРАВКИ ДАННЫХ ==========

/**
 * Отправляет данные измерений на сервер
 */
void sendMeasurements(WiFiClient& wifi) {
  // Проверяем, настроены ли API server и API key
  const char* apiServer = getApiServer();
  const char* apiKey = getApiKey();
  
  if (strlen(apiServer) == 0 || strlen(apiKey) == 0) {
    Serial.println("API configuration not set. Use 'apiserver <url>' and 'apikey <key>' commands.");
    return;
  }
  
  // Разбор URL для получения хоста и пути
  String apiUrl = String(apiServer);
  String host;
  String path;
  int port = 80;
  
  // Parse URL
  int protocolEnd = apiUrl.indexOf("://");
  if (protocolEnd > 0) {
    int hostStart = protocolEnd + 3;
    int pathStart = apiUrl.indexOf('/', hostStart);
    int portDelimiter = apiUrl.indexOf(':', hostStart);
    
    if (portDelimiter > 0 && portDelimiter < pathStart) {
      host = apiUrl.substring(hostStart, portDelimiter);
      port = apiUrl.substring(portDelimiter + 1, pathStart).toInt();
    } else {
      host = apiUrl.substring(hostStart, pathStart);
    }
    
    path = apiUrl.substring(pathStart);
  } else {
    Serial.println("Invalid API URL format. Should be http://host:port/path");
    return;
  }
  
  // Убедимся, что путь заканчивается на /bulk
  if (!path.endsWith("/bulk")) {
    // Если путь заканчивается на /, добавляем bulk
    if (path.endsWith("/")) {
      path += "bulk";
    } else {
      // Иначе добавляем /bulk
      path += "/bulk";
    }
  }
  
  // Создаем один объект JSON для всех измерений
  DynamicJsonDocument doc(1024);
  
  // Добавляем общую временную метку
  String timestamp = getCurrentTimeISO8601();
  doc["ts"] = timestamp;
  
  // Создаем массив для измерений
  JsonArray measurements = doc.createNestedArray("measurements");
  
  // Флаг, который показывает, получили ли мы хотя бы одно измерение
  bool hasValidMeasurements = false;
  
  // Получаем данные с датчика
  float dataBuffer[SENSOR_DATA_COUNT];
  if (readSensorData(dataBuffer, SENSOR_DATA_COUNT)) {
    // Для каждого значения от датчика создаем объект в массиве
    for (int j = 0; j < SENSOR_DATA_COUNT; j++) {
      JsonObject measurement = measurements.createNestedObject();
      measurement["sensor_id"] = SENSOR_LABELS[j];
      measurement["value"] = dataBuffer[j];
      hasValidMeasurements = true;
    }
  } else {
    Serial.println("Sensor read failed, skipping");
  }
  
  // Если нет данных, выходим
  if (!hasValidMeasurements) {
    Serial.println("No valid measurements to send");
    return;
  }
  
  // Сериализуем JSON в строку
  String payload;
  serializeJson(doc, payload);
  
  // Отладочная информация
  Serial.print("Sending payload:");
  Serial.println(payload);
  
  // Отправляем запрос
  HttpClient client(wifi, host.c_str(), port);
  client.beginRequest();
  client.post(path.c_str());
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("X-DEVICE-KEY", apiKey);
  client.sendHeader("Content-Length", payload.length());
  client.beginBody();
  client.print(payload);
  client.endRequest();
  
  // Получаем ответ
  int status = client.responseStatusCode();
  String response = client.responseBody();
  
  Serial.print("Status: "); Serial.println(status);
  Serial.print("Response: "); Serial.println(response);
}

// ========== ФУНКЦИИ ОБРАБОТКИ КОМАНД ==========

/**
 * Обработка команды из Serial
 */
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
    printSensorData();
  } else if (token && strcmp(token, "send") == 0) {
    sendMeasurements(wifiClient);
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

/**
 * Обработка ввода из Serial
 */
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

// ========== ФУНКЦИИ SETUP И LOOP ==========

void setup() {
  // Инициализация последовательного порта
  Serial.begin(115200);
  while (!Serial);  // Ждем готовности Serial порта

  // Загрузка конфигурации из EEPROM
  loadConfig();

  // Инициализация датчика
  initSensor();

  // Попытка подключения к WiFi, если есть конфигурация
  if (strlen(getWifiSSID()) > 0 && strlen(getWifiPassword()) > 0) {
    connectWiFi();
  }

  // Вывод информации о доступных командах
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
  // Обработка ввода команд из Serial
  processSerial();

  // Автоматическая отправка данных с интервалом
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= SEND_INTERVAL) {
    // Проверяем, настроены ли WiFi и API
    if (isConfigured() && WiFi.status() == WL_CONNECTED &&
        strlen(getApiServer()) > 0 && strlen(getApiKey()) > 0) {

      // Проверяем валидность времени RTC
      String now = getCurrentTimeISO8601();
      if (!now.startsWith("1970")) {
        Serial.println("Auto-send triggered");
        sendMeasurements(wifiClient);
      } else {
        Serial.println("RTC time invalid, skipping auto-send");
      }
    } else {
      // Логирование причины пропуска отправки
      if (!isConfigured()) {
        Serial.println("Device not configured, skipping auto-send");
      } else if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected, skipping auto-send");
      } else {
        Serial.println("API configuration missing, skipping auto-send");
      }
    }

    // Обновляем время последней отправки
    lastSendTime = currentTime;
  }

  // Небольшая задержка для стабильности
  delay(100);
}