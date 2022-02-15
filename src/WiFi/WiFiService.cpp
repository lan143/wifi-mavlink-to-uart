#include "WifiService.h"

WifiService::WifiService(Settings* settings)
{
    #ifdef ESP8266
        _wifi = new ESP8266WiFiClass();
    #elif defined(ESP32)
        _wifi = new WiFiClass();
    #endif

    _settings = settings;
}

void WifiService::init()
{
    String ssid = _settings->getWifiAPSSID();
    String password = _settings->getWifiAPPassword();

    _wifi->mode(WIFI_AP_STA);
    char ap_ssid[32];
    #ifdef ESP8266
        sprintf(ap_ssid, AP_SSID, ESP.getChipId());
    #elif defined(ESP32)
        sprintf(ap_ssid, AP_SSID, ESP.getEfuseMac());
    #endif
    _wifi->softAP(ap_ssid, AP_PASSWORD);

    if (!ssid.isEmpty() && !password.isEmpty()) {
        startClientMode(&ssid, &password);
    }
}

bool WifiService::startClientMode(String* ssid, String* password)
{
    _wifi->begin(ssid->c_str(), password->c_str());

    return true;
}

String WifiService::getWifiStatus()
{
    switch (_wifi->status()) {
        case WL_IDLE_STATUS:
            return "Idle";
        case WL_NO_SSID_AVAIL:
            return "No SSID available";
        case WL_CONNECTED:
            return "Connected";
        case WL_CONNECT_FAILED:
            return "Connection failed";
        case WL_CONNECTION_LOST:
            return "Connection lost";
        case WL_WRONG_PASSWORD:
            return "Wrong password";
        case WL_DISCONNECTED:
            return "Disconnected";
        default:
            return "Unknow status";
    }
}

std::list<String> WifiService::getWifiNetworks()
{
    //_wifi->mode(WIFI_STA);
    //_wifi->disconnect();

    int n = _wifi->scanNetworks();
    std::list<String> result;

    for (int i = 0; i < n; i++) {
        result.push_back(_wifi->SSID(i));
    }

    return result;
}
