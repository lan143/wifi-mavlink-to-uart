#include "SerialService.h"

SerialService::SerialService(HardwareSerial* serial)
{
    _serial = serial;
    _serial->begin(115200);
}

void SerialService::update()
{
    int availableBytes = _serial->available();

    if (availableBytes > 0) {
        uint8_t buffer[availableBytes];
        _serial->readBytes(&buffer[0], availableBytes);

        for (auto i = _onReadCallbacks.begin(); i != _onReadCallbacks.end(); i++) {
            (*i)(&buffer[0], availableBytes);
        }
    }
}

void SerialService::send(const uint8_t *buffer, size_t size)
{
    _serial->write(buffer, size);
}

void SerialService::onRead(std::function<void(uint8_t*, size_t)> callback)
{
    _onReadCallbacks.push_back(callback);
}
