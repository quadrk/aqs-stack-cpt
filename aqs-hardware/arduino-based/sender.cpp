#include "sender.h"
#include "config.h"
#include "wifi.h"

#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

void sendAllMeasurements(Sensor** sensors, int sensorCount, WiFiClient& wifi) {
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
  
  // Получаем данные со всех датчиков
  float dataBuffer[10];
  for (int i = 0; i < sensorCount; i++) {
    if (sensors[i]->read(dataBuffer, 10)) {
      // Для каждого значения от датчика создаем объект в массиве
      int count = sensors[i]->dataCount();
      const char** labels = sensors[i]->dataLabels();
      
      for (int j = 0; j < count; j++) {
        JsonObject measurement = measurements.createNestedObject();
        measurement["sensor_id"] = labels[j];
        measurement["value"] = dataBuffer[j];
        hasValidMeasurements = true;
      }
    } else {
      Serial.println("Sensor read failed, skipping");
    }
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
  Serial.println("Sending payload:");
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

void printSensorData(Sensor** sensors, int sensorCount) {
  float dataBuffer[10];
  for (int i = 0; i < sensorCount; i++) {
    if (sensors[i]->read(dataBuffer, 10)) {
      const char** labels = sensors[i]->dataLabels();
      int count = sensors[i]->dataCount();
      for (int j = 0; j < count; j++) {
        Serial.print(labels[j]);
        Serial.print(": ");
        Serial.println(dataBuffer[j]);
      }
    } else {
      Serial.println("No data available");
    }
  }
}