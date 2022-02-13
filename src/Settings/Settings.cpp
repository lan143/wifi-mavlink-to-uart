#include "Settings.h"
#include <Arduino.h>

Settings::Settings()
{
    _eeprom = new EEPROMClass();
    _eeprom->begin(sizeof(_settings));
    loadSettings();
}

void Settings::loadSettings()
{
    _settings = _eeprom->get(0x0, _settings);

    if (_settings.version == 0xff) {
        initDefaultSettings();
    }
}

void Settings::initDefaultSettings()
{
    _settings.version = 1;
    _settings.mavlinkPort = 8888;
    sprintf(&_settings.apSSID[0], "");
    sprintf(&_settings.apPassword[0], "");
    saveSettings();
}

void Settings::saveSettings()
{
    _eeprom->put(0x0, _settings);
    _eeprom->commit();
}

void Settings::setMavlinkPort(uint16_t port)
{
    _settings.mavlinkPort = port;
    saveSettings();
}

void Settings::setWifiAPSSID(String ssid)
{
    if (ssid.length() > 31) {
        assert(true);
    }

    sprintf(&_settings.apSSID[0], "%s", ssid.c_str());
    saveSettings();
}

void Settings::setWifiAPPassword(String password)
{
    if (password.length() > 63) {
        assert(true);
    }

    sprintf(&_settings.apPassword[0], "%s", password.c_str());
    saveSettings();
}

uint16_t Settings::getMavlinkPort()
{
    return _settings.mavlinkPort;
}

String Settings::getWifiAPSSID()
{
    return String(_settings.apSSID);
}

String Settings::getWifiAPPassword()
{
    return String(_settings.apPassword);
}
