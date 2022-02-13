#pragma once

#ifdef ESP32
    #include <AsyncTCP.h>
#elif defined(ESP8266)
    #include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include "WiFi/WiFiService.h"
#include "Settings/Settings.h"

class Web {
public:
    Web(WifiService* wifiService, Settings* settings);

private:
    AsyncWebServer* _server;
    WifiService* _wifiService;
    Settings* _settings;
};