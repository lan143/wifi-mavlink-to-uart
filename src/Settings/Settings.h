#pragma once

#include <EEPROM.h>
#include <WString.h>

enum {
    ADDRESS_MAVLINK_PORT = 0x01,
    ADDRESS_WIFI_SSID = 0x03,
    ADDRESS_WIFI_PASSWORD = 0x30,
    ADDRESS_MAX = 0x200,
};

class Settings {
public:
    Settings();

    void setWifiAPSSID(String ssid);
    void setWifiAPPassword(String password);
    void setMavlinkPort(uint16_t port);

    String getWifiAPSSID();
    String getWifiAPPassword();
    uint16_t getMavlinkPort();

private:
    void saveString(uint16_t from, uint16_t to, String string);
    String getString(uint16_t from, uint16_t to);

private:
    EEPROMClass *_eeprom;
};