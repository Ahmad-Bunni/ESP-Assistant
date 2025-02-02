#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

#define LED_PIN 21

class LEDManager
{
public:
    static void initialize();
    static void setConnectionStatus(bool connected);
    static void setRequestInProgress(bool inProgress);
};

#endif // LED_MANAGER_H
