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

    void update();

    std::list<String> getWifiNetworks();

protected:
    bool _wifiLose;

    bool startClientMode(String* ssid, String* password);

    void checkWifiStatus();
    void reconnectToWifiIfNeeded();

    unsigned long _lastReconnect = 0;

    #ifdef ESP8266
        ESP8266WiFiClass *_wifi;
    #elif defined(ESP32)
        WiFiClass *_wifi;
    #endif

    Settings* _settings;
};
