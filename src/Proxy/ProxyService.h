#pragma once

#include <Arduino.h>
#include "Serial/SerialService.h"
#include "UDP/MavlinkUDP.h"

class ProxyService {
public:
    ProxyService(HardwareSerial* serial);
    void start();

    void update();

private:
    SerialService* _serial;
    MavlinkUDP* _udp;
};
