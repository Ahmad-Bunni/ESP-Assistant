#include "core/wifi/wifi_manager.h"
#include "features/ai/openai_client.h"
#include "features/led/led_manager.h"
#include <Arduino.h>

String inputString = "";     // String to hold incoming data
bool stringComplete = false; // Whether the string is complete

void setup()
{
  Serial.begin(115200);
  // Wait for serial connection for up to 10 seconds
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime) < 10000)
  {
    ; // Wait for serial port to connect
  }

  // Initialize LED manager
  LEDManager::initialize();

  // Reserve 200 bytes for the inputString
  inputString.reserve(200);

  // Connect to WiFi
  WiFiManager::connect();

  Serial.println("Ready for input. Type your message and press enter.");
}

void loop()
{
  // Process complete messages
  if (stringComplete)
  {
    Serial.println("Received: " + inputString);

    if (inputString.length() > 0)
    {
      Serial.println("Making API request...");
      String response = OpenAIClient::makeRequest(inputString);
      Serial.println("Response from OpenAI:");
      Serial.println(response);
    }

    // Clear the string for next time
    inputString = "";
    stringComplete = false;
  }

  // Read from Serial
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    else
    {
      inputString += inChar;
    }
  }
}
