WiFi MAVlink to uart
===

This device designed for route MAVlink telemetry from TBS Crossfire WiFi connection to ground station with UART connection. Device used ESP8266 for works.

Installing
---
With Platformio:
1. Download and install Platformio IDE
2. Clone or download project code
3. Open project in Platformion IDE
4. Attach your ESP8266 with usb or usb-uart adapter.
5. Click on PlatformIO -> Generar -> Upload. Wait uploading firmware.
6. Click on PlatformIO -> Platform -> Upload Filesystem Image.

With buildin image:
1. Go to release tab, download firmware.bin and spiffs.bin
2. Flash firmware.bin and spiffs.bin to your ESP8266

Launch:
1. Attach your ESP8266 with UART to your ground station.
2. Power up ESP8266, connect with WiFi to wifi-mavlink-to-uart-0xXXXXXX ssid. Password: 1234567890
3. Go to 192.168.4.1 and setup wifi connection.
4. Restart ESP8266.
5. Setup UART speed in your ground station to 115200 and select MAVlink protocol.
6. Setup MAVlink support in your TBS Crossfire RX and TX.

Youre welcome!