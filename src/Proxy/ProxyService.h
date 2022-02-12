#pragma once

#include <Arduino.h>
#include "Settings/Settings.h"
#include "Serial/SerialService.h"
#include "UDP/MavlinkUDP.h"

class ProxyService {
public:
    ProxyService(HardwareSerial* serial, Settings* settings);
    void start();

    void update();

private:
    SerialService* _serial;
    MavlinkUDP* _udp;
};
