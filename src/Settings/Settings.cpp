#include "Settings.h"
#include <Arduino.h>

Settings::Settings()
{
    _eeprom = new EEPROMClass();
    _eeprom->begin(ADDRESS_MAX);
}

void Settings::setMavlinkPort(uint16_t port)
{
    _eeprom->put(ADDRESS_MAVLINK_PORT, port);
    _eeprom->commit();
}

void Settings::setWifiAPSSID(String ssid)
{
    saveString(ADDRESS_WIFI_SSID, ADDRESS_WIFI_PASSWORD, ssid);
}

void Settings::setWifiAPPassword(String password)
{
    saveString(ADDRESS_WIFI_PASSWORD, ADDRESS_MAX, password);
}

uint16_t Settings::getMavlinkPort()
{
    int16_t port;
    port = _eeprom->get(ADDRESS_MAVLINK_PORT, port);

    return port ?: 8888;
}

String Settings::getWifiAPSSID()
{
    return getString(ADDRESS_WIFI_SSID, ADDRESS_WIFI_PASSWORD);
}

String Settings::getWifiAPPassword()
{
    return getString(ADDRESS_WIFI_PASSWORD, ADDRESS_MAX);
}

void Settings::saveString(uint16_t from, uint16_t to, String string)
{
    if (sizeof(string) > (to - from)) {
        return;
    }

    uint16_t i;
    uint16_t j = 0;

    for (i = from; i < from + sizeof(string); i++) {
        _eeprom->write(i, string[j++]);
    }

    _eeprom->write(++i, '\0');
    _eeprom->commit();
}

String Settings::getString(uint16_t from, uint16_t to)
{
    String string = "";

    for (int16_t i = from; i < to; i++) {
        char byte = this->_eeprom->read(i);

        if (byte != '\0') {
            string += byte;
        } else {
            break;
        }
    }

    return string;
}
