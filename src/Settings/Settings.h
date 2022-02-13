#pragma once

#include <EEPROM.h>
#include <WString.h>

struct SettingsStruct {
    uint8_t version;
    uint16_t mavlinkPort;
    char apSSID[32];
    char apPassword[64];
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
    void loadSettings();
    void initDefaultSettings();
    void saveSettings();

private:
    EEPROMClass *_eeprom;
    SettingsStruct _settings;
};