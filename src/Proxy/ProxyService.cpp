#include "ProxyService.h"

ProxyService::ProxyService(HardwareSerial* serial, Settings* settings)
{
    _serial = new SerialService(serial);
    _udp = new MavlinkUDP(settings->getMavlinkPort());

    _serial->onRead([this](uint8_t* data, size_t size) {
        this->_udp->send(data, size);
    });
    _udp->onRead([this](uint8_t* data, size_t size) {
        this->_serial->send(data, size);
    });
}

void ProxyService::start()
{
    _udp->start();
}

void ProxyService::update()
{
    _serial->update();
}
