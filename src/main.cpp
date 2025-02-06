#include "core/wifi/wifi_manager.h"
#include "features/ai/openai_client.h"
#include "features/audio/audio_service.h"
#include "features/led/led_manager.h"
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);

  // Initialize core services
  if (!AudioService::setup())
  {
    Serial.println("Audio init failed");
    return;
  }

  LEDManager::initialize();
  WiFiManager::connect();
}

void loop()
{
  if (Serial.available())
  {
    while (Serial.available())
      Serial.read();

    // Record audio
    LEDManager::setRequestInProgress(true);
    AudioService::record();

    // Process with OpenAI
    String transcription = OpenAIClient::transcribeAudio(
        AudioService::getBuffer(),
        AudioService::getBufferSize());

    String response = OpenAIClient::makeRequest(transcription);
    Serial.println(response);

    LEDManager::setRequestInProgress(false);
  }
  delay(10);
}
