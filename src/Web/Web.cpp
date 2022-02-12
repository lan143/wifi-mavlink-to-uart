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
        if (request->hasParam("wifiSSID", true) && request->hasParam("wifiPassword", true)) {
            AsyncWebParameter* wifiSSID = request->getParam("wifiSSID", true);
            AsyncWebParameter* wifiPassword = request->getParam("wifiPassword", true);

            _settings->setWifiAPSSID(wifiSSID->value());
            _settings->setWifiAPPassword(wifiPassword->value());

            request->send(200, "application/json", "{}");
        } else {
            request->send(422, "application/json", "{\"message\": \"empty wifiSSID or wifiPassword\"}");
        }
    });

    _server->on("/api/settings", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("mavlinkPort", true)) {
            AsyncWebParameter* mavlinkPort = request->getParam("mavlinkPort", true);

            _settings->setMavlinkPort(atoi(mavlinkPort->value().c_str()));

            request->send(200, "application/json", "{}");
        } else {
            request->send(422, "application/json", "{\"message\": \"empty mavlinkPort\"}");
        }
    });

    _server->begin();
}
