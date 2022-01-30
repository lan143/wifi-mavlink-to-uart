#include <Arduino.h>
#include "Proxy/ProxyService.h"
#include "Web/Web.h"
#include "WiFi/WiFiService.h"
#include "Settings/Settings.h"

#ifdef ESP32
    #include <SPIFFS.h>
#endif

ProxyService* proxy;
Web* web;
WifiService* wifiService;
Settings* settings;

void setup()
{
    SPIFFS.begin();

    settings = new Settings();

    wifiService = new WifiService(settings);
    wifiService->init();

    proxy = new ProxyService(&Serial);
    proxy->start();

    web = new Web(wifiService, settings);
}

void loop()
{
    wifiService->update();
    proxy->update();
}
