#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager
{
public:
    static void connect();
    static bool isConnected();

private:
    static void printWiFiStatus();
};

#endif // WIFI_MANAGER_H