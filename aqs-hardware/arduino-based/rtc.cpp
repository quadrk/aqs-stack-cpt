#include "rtc.h"

#include <RTC.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP udp;
NTPClient timeClient(udp, "europe.pool.ntp.org", 0, 60000); // обновление каждую минуту

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
                delay(2000);
                RTCTime rtcTime(epoch);
                delay(2000);
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

    Serial.println("Failed to sync time via NTP after multiple attempts. Rebooting the device.");
    delay(3000);
    NVIC_SystemReset();
}