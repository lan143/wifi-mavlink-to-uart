#pragma once

#include <Arduino.h>
#include <list>

#ifdef ESP8266
    #include <ESPAsyncUDP.h>
#elif defined(ESP32)
    #include <AsyncUDP.h>
#endif

struct Endpoint {
    IPAddress remoteIp;
    uint16_t remotePort;
};

class MavlinkUDP {
public:
    MavlinkUDP(int port);
    bool start();
    void send(uint8_t* data, size_t size);
    void onRead(std::function<void(uint8_t*, size_t)> callback);

private:
    void onPacket(AsyncUDPPacket* packet);

private:
    AsyncUDP _client;
    int _port;
    std::list<Endpoint> _endpoints;
    std::list<std::function<void(uint8_t*, size_t)>> _onReadCallbacks;
};
