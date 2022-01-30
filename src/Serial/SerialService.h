#pragma once

#include <Arduino.h>
#include <list>

class SerialService {
public:
    SerialService(HardwareSerial* serial);

    void update();
    void send(const uint8_t *buffer, size_t size);
    void onRead(std::function<void(uint8_t*, size_t)> callback);

private:
    HardwareSerial* _serial;
    std::list<std::function<void(uint8_t*, size_t)>> _onReadCallbacks;
};
