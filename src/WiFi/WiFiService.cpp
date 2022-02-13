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

void WifiService::update()
{
    checkWifiStatus();
    reconnectToWifiIfNeeded();
}

bool WifiService::startClientMode(String* ssid, String* password)
{
    _wifi->begin(ssid->c_str(), password->c_str());

    int numberOfAttempts = 0;

    while (_wifi->status() != WL_CONNECTED) {
        numberOfAttempts++;

        delay(500);

        if (numberOfAttempts > 20) {
            break;
        }
    }

    if (_wifi->status() == WL_CONNECTED) {
        _wifiLose = false;

        return true;
    } else {
        _wifiLose = true;

        return false;
    }
}

void WifiService::checkWifiStatus()
{
    if (_wifi->status() != WL_CONNECTED && !_wifiLose) {
        _wifiLose = true;
    } else if (_wifi->status() == WL_CONNECTED && _wifiLose) {
        _wifiLose = false;
    }
}

void WifiService::reconnectToWifiIfNeeded()
{
    if (_wifiLose) {
        if ((millis() - _lastReconnect) >= RECONNECT_INTERVAL) {
            _wifi->reconnect();
            _lastReconnect = millis();
        }
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
