#include "wifi_manager.h"
#include <Arduino.h>

void WiFiManager::connect()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool WiFiManager::isConnected()
{
    bool connected = WiFi.status() == WL_CONNECTED;
    return connected;
}
