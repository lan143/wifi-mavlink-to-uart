#include "Web.h"
#include <ArduinoJSON.h>

Web::Web(WifiService* wifiService, Settings* settings)
{
    _wifiService = wifiService;
    _settings = settings;

    _server = new AsyncWebServer(80);

    _server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    _server->serveStatic("/jquery-3.6.0.min.js", SPIFFS, "/jquery-3.6.0.min.js");
    _server->serveStatic("/bootstrap.min.js", SPIFFS, "/bootstrap.min.js");

    _server->on("/api/wifi/list", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument json(200);

        auto networks = this->_wifiService->getWifiNetworks();

        for (auto i = networks.begin(); i != networks.end(); i++) {
            json.add(*i);
        }

        serializeJson(json, *response);

        request->send(response);
    });

    _server->on("/api/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument json(200);
        json["wifiSSID"] = _settings->getWifiAPSSID();
        json["wifiPassword"] = _settings->getWifiAPPassword();
        json["mavlinkPort"] = _settings->getMavlinkPort();

        serializeJson(json, *response);

        request->send(response);
    });

    _server->on("/api/settings/wifi", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (!request->hasParam("wifiSSID", true) || !request->hasParam("wifiPassword", true)) {
            request->send(422, "application/json", "{\"message\": \"not present wifiSSID or wifiPassword in request\"}");
            return;
        }

        AsyncWebParameter* wifiSSID = request->getParam("wifiSSID", true);
        AsyncWebParameter* wifiPassword = request->getParam("wifiPassword", true);

        if (wifiSSID->value().length() > 31) {
            request->send(422, "application/json", "{\"message\": \"WiFi SSID lenght more 31 symbols\"}");
            return;
        }

        if (wifiPassword->value().length() > 63) {
            request->send(422, "application/json", "{\"message\": \"WiFi password lenght more 63 symbols\"}");
            return;
        }

        _settings->setWifiAPSSID(wifiSSID->value());
        _settings->setWifiAPPassword(wifiPassword->value());

        request->send(200, "application/json", "{}");
    });

    _server->on("/api/settings", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (!request->hasParam("mavlinkPort", true)) {
            request->send(422, "application/json", "{\"message\": \"not present mavlinkPort in request\"}");
            return;
        }

        AsyncWebParameter* mavlinkPort = request->getParam("mavlinkPort", true);
        int mavlinkPortValue = atoi(mavlinkPort->value().c_str());

        if (mavlinkPortValue < 1 || mavlinkPortValue > 65535) {
            request->send(422, "application/json", "{\"message\": \"MavLINK port should be more 1 and less 65535\"}");
            return;
        }

        _settings->setMavlinkPort(mavlinkPortValue);

        request->send(200, "application/json", "{}");
    });

    _server->on("/api/reboot", HTTP_POST, [this](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{}");
        ESP.restart();
    });

    _server->begin();
}
