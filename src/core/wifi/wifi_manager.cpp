#include "wifi_manager.h"
#include "features/led/led_manager.h"
#include <Arduino.h>

void WiFiManager::connect()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        LEDManager::setConnectionStatus(false);
    }

    LEDManager::setConnectionStatus(true);
    printWiFiStatus();
}

bool WiFiManager::isConnected()
{
    bool connected = WiFi.status() == WL_CONNECTED;
    LEDManager::setConnectionStatus(connected);
    return connected;
}

void WiFiManager::printWiFiStatus()
{
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}