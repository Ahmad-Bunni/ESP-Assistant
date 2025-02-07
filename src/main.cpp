#include "core/wifi/wifi_manager.h"
#include "features/ai/openai_client.h"
#include "features/audio/audio_service.h"
#include "features/led/led_manager.h"
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);

  AudioService::setup();
  LEDManager::initialize();
  WiFiManager::connect();
}

void loop()
{
  if (Serial.available())
  {
    while (Serial.available())
      Serial.read();

    LEDManager::setRequestInProgress(true);
    AudioService::record();

    String transcription = OpenAIClient::transcribeAudio(
        AudioService::getBuffer(),
        AudioService::getBufferSize());

    String response = OpenAIClient::makeRequest(transcription);
    Serial.println(response);

    LEDManager::setRequestInProgress(false);
  }
  delay(10);
}
