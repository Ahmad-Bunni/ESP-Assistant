#include "led_manager.h"
#include <Adafruit_NeoPixel.h>
#include <atomic>

#define NUM_PIXELS 1 // Single WS2812 LED

Adafruit_NeoPixel ledStrip(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void LEDManager::initialize()
{
    ledStrip.begin();
    ledStrip.show(); // Turn off all LEDs initially
}

void LEDManager::setRequestInProgress(bool inProgress)
{
    if (inProgress)
    {
        ledStrip.setPixelColor(0, ledStrip.Color(0, 0, 255)); // Blue for in progress
    }
    else
    {
        ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0)); // Green
    }

    ledStrip.show();
}

void LEDManager::setConnectionStatus(bool connected)
{
    if (connected)
    {
        ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0)); // Green for connected
    }
    else
    {
        ledStrip.setPixelColor(0, ledStrip.Color(255, 0, 0)); // Red for disconnected
    }

    ledStrip.show();
}
