#include "MavlinkUDP.h"

MavlinkUDP::MavlinkUDP(int port)
{
    _port = port;
}

bool MavlinkUDP::start()
{
    IPAddress addr(0, 0, 0, 0);

    if (_client.listen(addr, _port)) {
        _client.onPacket([this](AsyncUDPPacket packet) {
            this->onPacket(&packet);
        });

        return true;
    }

    return false;
}

void MavlinkUDP::onPacket(AsyncUDPPacket* packet)
{
    bool found = false;
    for (auto i = _endpoints.begin(); i != _endpoints.end(); i++) {
        if ((*i).remoteIp.toString() == packet->remoteIP().toString()
            && (*i).remotePort == packet->remotePort()
        ) {
            found = true;
        }
    }

    if (!found) {
        Endpoint endpoint;
        endpoint.remoteIp = packet->remoteIP();
        endpoint.remotePort = packet->remotePort();
        _endpoints.push_back(endpoint);
    }

    for (auto i = _onReadCallbacks.begin(); i != _onReadCallbacks.end(); i++) {
        (*i)(packet->data(), packet->length());
    }
}

void MavlinkUDP::send(uint8_t* data, size_t size)
{
    AsyncUDPMessage message;
    message.write(data, size);

    for (auto i = _endpoints.begin(); i != _endpoints.end(); i++) {
        _client.sendTo(message, (*i).remoteIp, (*i).remotePort);
    }
}

void MavlinkUDP::onRead(std::function<void(uint8_t*, size_t)> callback)
{
    _onReadCallbacks.push_back(callback);
}
