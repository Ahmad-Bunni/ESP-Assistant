#ifndef OPENAI_CLIENT_H
#define OPENAI_CLIENT_H

#include "features/audio/audio_service.h"
#include <Arduino.h>

class OpenAIClient
{
public:
    static String makeRequest(const String &userInput);
    static String transcribeAudio(const int16_t *audioBuffer, size_t bufferSize);

private:
    static String createRequestPayload(const String &userInput);
    static String extractMessageFromResponse(const String &response);
    static void createWavHeader(uint8_t *header, int dataSize);
};

#endif