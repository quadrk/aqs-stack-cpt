/*
 *  Waspmote Smart Agriculture: Weather Station TCP Uploader
 *  (v1.3 — 02.05.2025)
 *  — удалена передача device_id, теперь он подставляется на сервере по X-DEVICE-KEY
 */

 #include <WaspWIFI.h>
 #include <WaspSensorAgr_v20.h>
 #include <ctype.h>
 #include <stdio.h>
 #include <string.h>
 
 // ----- Wi-Fi настройки -----
 uint8_t  socket       = SOCKET0;
 char     ESSID[]      = "fish";
 char     AUTHKEY[]    = "";
 int8_t   GMT_OFFSET   = 0;
 
 // ----- Сервер -----
 char     HOST[]       = "192.168.100.32";
 char     URL[]        = "/api/v1/devices/measurements/";
 const uint16_t REMOTE_PORT = 8000;
 const uint16_t LOCAL_PORT  = 6000;
 
 // ----- Временные буферы -----
 float pressureValue, windValue, rain1, rain2, rain3;
 char pressureStr[8], windStr[8], rain1Str[8], rain2Str[8], rain3Str[8], vaneStr[8];
 char payload[240], httpRequest[400];
 uint8_t status;
 
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
     "\"ts\":\"%04u-%02u-%02uT%02u:%02u:%02uZ\",\"value\":\"%s\"}",
     sensor_id, year4,mm,dd, hh,mi,ss, val_str
   );
 
   USB.print(F("Payload: "));
   USB.println(payload);
 
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
     "X-DEVICE-KEY: 3b92b21414de57056db67f7237a044a239fd197757e0d197fcaf62afeac05afd\r\n"
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
   USB.ON(); USB.println(F("Start AGRI TCP uploader")); delay(100);
   SensorAgrv20.ON();
   sync_time_ntp();
   wifi_setup_tcp();
 }
 
 void loop() {
   bool joined = false;
   for (uint8_t a = 1; a <= 3; ++a) {
     WIFI.OFF(); delay(500);
     WIFI.reset(); delay(500);
 
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
 
   SensorAgrv20.setSensorMode(SENS_ON, SENS_AGR_PRESSURE); delay(100);
   pressureValue = SensorAgrv20.readValue(SENS_AGR_PRESSURE);
   SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_PRESSURE);
 
   SensorAgrv20.setSensorMode(SENS_ON, SENS_AGR_ANEMOMETER); delay(10);
   windValue = SensorAgrv20.readValue(SENS_AGR_ANEMOMETER);
   SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_ANEMOMETER);
 
   rain1 = SensorAgrv20.readPluviometerCurrent();
   rain2 = SensorAgrv20.readPluviometerHour();
   rain3 = SensorAgrv20.readPluviometerDay();
 
   switch (SensorAgrv20.vaneDirection) {
     case SENS_AGR_VANE_N:    snprintf(vaneStr, sizeof(vaneStr), "0"); break;
     case SENS_AGR_VANE_NNE:  snprintf(vaneStr, sizeof(vaneStr), "22"); break;
     case SENS_AGR_VANE_NE:   snprintf(vaneStr, sizeof(vaneStr), "45"); break;
     case SENS_AGR_VANE_ENE:  snprintf(vaneStr, sizeof(vaneStr), "67"); break;
     case SENS_AGR_VANE_E:    snprintf(vaneStr, sizeof(vaneStr), "90"); break;
     case SENS_AGR_VANE_ESE:  snprintf(vaneStr, sizeof(vaneStr), "112"); break;
     case SENS_AGR_VANE_SE:   snprintf(vaneStr, sizeof(vaneStr), "135"); break;
     case SENS_AGR_VANE_SSE:  snprintf(vaneStr, sizeof(vaneStr), "157"); break;
     case SENS_AGR_VANE_S:    snprintf(vaneStr, sizeof(vaneStr), "180"); break;
     case SENS_AGR_VANE_SSW:  snprintf(vaneStr, sizeof(vaneStr), "202"); break;
     case SENS_AGR_VANE_SW:   snprintf(vaneStr, sizeof(vaneStr), "225"); break;
     case SENS_AGR_VANE_WSW:  snprintf(vaneStr, sizeof(vaneStr), "247"); break;
     case SENS_AGR_VANE_W:    snprintf(vaneStr, sizeof(vaneStr), "270"); break;
     case SENS_AGR_VANE_WNW:  snprintf(vaneStr, sizeof(vaneStr), "292"); break;
     case SENS_AGR_VANE_NW:   snprintf(vaneStr, sizeof(vaneStr), "315"); break;
     case SENS_AGR_VANE_NNW:  snprintf(vaneStr, sizeof(vaneStr), "337"); break;
     default:                 snprintf(vaneStr, sizeof(vaneStr), "999"); break;
   }
 
   dtostrf(pressureValue, 4, 2, pressureStr);
   dtostrf(windValue, 4, 2, windStr);
   dtostrf(rain1, 4, 2, rain1Str);
   dtostrf(rain2, 4, 2, rain2Str);
   dtostrf(rain3, 4, 2, rain3Str);
 
   send_measurement("PRESSURE", pressureStr);
   send_measurement("WIND", windStr);
   send_measurement("RAIN_HOUR", rain1Str);
   send_measurement("RAIN_PREV_HOUR", rain2Str);
   send_measurement("RAIN_DAY", rain3Str);
   send_measurement("WIND_DIR", vaneStr);
 
   WIFI.leave();
   WIFI.OFF();
 
   USB.println(F("Done! Waiting before restart..."));
   delay(10000);
 }
 