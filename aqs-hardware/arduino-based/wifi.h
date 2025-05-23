#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>

void connectWiFi();
void reconnectWiFi(const char* ssid, const char* password);
String getCurrentTimeISO8601();

#endif
