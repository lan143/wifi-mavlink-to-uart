#pragma once

#ifdef ESP8266
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#endif

#include <list>
#include "Settings/Settings.h"

#define RECONNECT_INTERVAL 120 * 1000
#define AP_SSID "wifi-mavlink-to-uart-%#010x"
#define AP_PASSWORD "1234567890"

class WifiService {
public:
    WifiService(Settings* settings);

    void init();

    std::list<String> getWifiNetworks();
    String getWifiStatus();

protected:
    bool startClientMode(String* ssid, String* password);

    #ifdef ESP8266
        ESP8266WiFiClass *_wifi;
    #elif defined(ESP32)
        WiFiClass *_wifi;
    #endif

    Settings* _settings;
};
