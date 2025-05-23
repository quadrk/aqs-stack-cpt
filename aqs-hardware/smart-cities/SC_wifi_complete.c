/*
 *  Waspmote Smart Cities: Dust, Temperature, Humidity, Luminance & Noise TCP Uploader
 *  (v1.2 — 02.05.2025)
 *  — удалена передача device_id, теперь он подставляется на сервере по X-DEVICE-KEY
 */

 #include <WaspWIFI.h>
 #include <WaspSensorCities.h>
 #include <ctype.h>
 #include <stdio.h>
 #include <string.h>
 
 // ----- Wi-Fi настройки -----
 uint8_t  socket       = SOCKET0;
 char     ESSID[]      = "fish";
 char     AUTHKEY[]    = "Qv3Ce6jNg5Jn64ZjZmM7XWX3mWkcfMt3gEQm9CsU5we5JgDyB6v2fsRYqVSs"; // WPA2 password
 int8_t   GMT_OFFSET   = 0;
 
 // ----- Сервер -----
 char     HOST[]       = "192.168.100.32";  // ← локальный IP сервера
 char     URL[]        = "/api/v1/devices/measurements/";
 const uint16_t REMOTE_PORT = 8000;
 const uint16_t LOCAL_PORT  = 6000;
 
 // ----- Временные буферы -----
 float    dustValue, tempValue, humidityValue, ldrValue, soundValue;
 char     dustStr[8], tempStr[8], rhStr[8], ldrStr[8], soundStr[8];
 char     payload[240], httpRequest[400];
 uint8_t  status;
 
 // --- Синхронизация времени через NTP ---
 void sync_time_ntp() {
   if (WIFI.ON(socket) != 1) return;
   WIFI.setConnectionOptions(UDP);
   WIFI.setDHCPoptions(DHCP_ON);
   WIFI.setJoinMode(MANUAL);
   WIFI.setAuthKey(WPA2, AUTHKEY);
   WIFI.storeData();
   RTC.ON();
   RTC.setGMT(GMT_OFFSET);
 
   if (WIFI.join(ESSID)) {
     if (WIFI.setRTCfromWiFi() == 0) {
       USB.print(F("RTC synced: "));
       USB.println(RTC.getTime());
     }
     WIFI.leave();
   }
   WIFI.OFF();
 }
 
 // --- Настройка TCP Wi-Fi ---
 void wifi_setup_tcp() {
   if (WIFI.ON(socket) != 1) return;
   WIFI.setConnectionOptions(CLIENT_SERVER);
   WIFI.setDHCPoptions(DHCP_ON);
   WIFI.setJoinMode(MANUAL);
   WIFI.setAuthKey(WPA2, AUTHKEY);
   WIFI.storeData();
 }
 
 // --- Отправка одного измерения ---
 void send_measurement(const char *sensor_id, const char *val_str) {
   char *rtc_str = RTC.getTime();
   unsigned int yy, mm, dd, hh, mi, ss;
   sscanf(rtc_str, "%*[^,], %2u/%2u/%2u, %2u:%2u:%2u", &yy,&mm,&dd,&hh,&mi,&ss);
   uint16_t year4 = 2000 + yy;
 
   snprintf(payload, sizeof(payload),
     "{\"sensor_id\":\"%s\","
     "\"ts\":\"%04u-%02u-%02uT%02u:%02u:%02uZ\",\"value\":%s}",
     sensor_id, year4,mm,dd, hh,mi,ss, val_str
   );
   USB.print(F("Payload: "));
   USB.println(payload);

    // TCP retry
   bool ok = false;
   for (uint8_t a = 1; a <= 3; ++a) {
     status = WIFI.setTCPclient(HOST, REMOTE_PORT, LOCAL_PORT);
     if (status == 1) { ok = true; break; }
     USB.print(F("TCP fail, attempt "));
     USB.println(a);
     WIFI.close(); delay(1000);
   }
   if (!ok) return;
 
   uint16_t len = strlen(payload);
   snprintf(httpRequest, sizeof(httpRequest),
     "POST %s HTTP/1.1\r\n"
     "Host: %s:%u\r\n"
     "Content-Type: application/json\r\n"
     "Content-Length: %u\r\n"
     "X-DEVICE-KEY: 364d2174501c1cafb5dd1e2fff0b8aeae17490ed6254ad47685451e5abcfece6\r\n" // DEVICE API KEY (created on front)
     "Connection: close\r\n\r\n"
     "%s",
     URL, HOST, REMOTE_PORT, len, payload
   );
 
   WIFI.send((uint8_t*)httpRequest, strlen(httpRequest));
   delay(200);
 
   if (WIFI.read(NOBLO) > 0) {
     USB.print(F("Response: "));
     for (int i = 0; i < WIFI.length; ++i)
       USB.print(WIFI.answer[i], BYTE);
     USB.println();
   }
 
   WIFI.close();
 }
 
 void setup() {
   USB.ON(); USB.println(F("Start SC TCP uploader")); delay(100);
   SensorCities.ON();
   sync_time_ntp();
   wifi_setup_tcp();
 }
 
 void loop() {
   bool joined = false;
   for (uint8_t a = 1; a <= 3; ++a) {
     WIFI.OFF();  // гарантированное отключение
     delay(500);
     WIFI.reset(); // сброс стека модуля
     delay(500);
 
     if (WIFI.ON(socket) != 1) continue;
     if (WIFI.join(ESSID)) { joined = true; break; }
 
     USB.print(F("WiFi join fail ")); USB.println(a);
     delay(1000);
   }
 
   if (!joined) {
     USB.println(F("Join failed, skipping"));
     return;
   }
 
   WIFI.getIP(); USB.println(F("Joined AP"));
 
   // Считывание пыли
   SensorCities.setSensorMode(SENS_ON, SENS_CITIES_DUST); delay(2000);
   dustValue = SensorCities.readValue(SENS_CITIES_DUST);
   SensorCities.setSensorMode(SENS_OFF, SENS_CITIES_DUST);
 
   // Считывание температуры
   SensorCities.setSensorMode(SENS_ON, SENS_CITIES_TEMPERATURE); delay(10);
   tempValue = SensorCities.readValue(SENS_CITIES_TEMPERATURE);
   SensorCities.setSensorMode(SENS_OFF, SENS_CITIES_TEMPERATURE);
 
   // Считывание влажности
   SensorCities.setSensorMode(SENS_ON, SENS_CITIES_HUMIDITY); delay(15000);
   humidityValue = SensorCities.readValue(SENS_CITIES_HUMIDITY);
   SensorCities.setSensorMode(SENS_OFF, SENS_CITIES_HUMIDITY);
 
   // Считывание освещённости (LDR)
   SensorCities.setSensorMode(SENS_ON, SENS_CITIES_LDR); delay(10);
   ldrValue = SensorCities.readValue(SENS_CITIES_LDR);
   SensorCities.setSensorMode(SENS_OFF, SENS_CITIES_LDR);
 
   // Считывание звука
   SensorCities.setSensorMode(SENS_ON, SENS_CITIES_AUDIO); delay(2000);
   soundValue = SensorCities.readValue(SENS_CITIES_AUDIO);
   SensorCities.setSensorMode(SENS_OFF, SENS_CITIES_AUDIO);
 
   // Преобразование значений в строки
   dtostrf(dustValue, 4, 2, dustStr);
   dtostrf(tempValue, 4, 2, tempStr);
   dtostrf(humidityValue, 4, 2, rhStr);
   dtostrf(ldrValue, 4, 2, ldrStr);
   dtostrf(soundValue, 4, 2, soundStr);
 
   // Отправка
   send_measurement("PM25", dustStr);
   send_measurement("T", tempStr);
   send_measurement("RH", rhStr);
   send_measurement("LUX", ldrStr);
   send_measurement("NOISE", soundStr);
 
   WIFI.leave();
   WIFI.OFF();
 
   USB.println(F("Done! Waiting before restart..."));
   delay(10000);
 }
 