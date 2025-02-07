#pragma once

#include "features/audio/audio_service.h"
#include <Arduino.h>
#include <memory>

class MultipartFormBuilder
{
public:
    struct FormData
    {
        String boundary;
        String contentType;
        std::unique_ptr<uint8_t[]> payload;
        size_t totalLength;
    };

    static FormData createAudioForm(const int16_t *audioBuffer, size_t bufferSize);
};

class OpenAIClient
{
public:
    static String makeRequest(const String &userInput);
    static String transcribeAudio(const int16_t *audioBuffer, size_t bufferSize);

private:
    static String handleApiResponse(int httpCode, const String &response);
    static String createRequestPayload(const String &userInput);
    static String extractMessageFromResponse(const String &response);
    static void createWavHeader(uint8_t *header, int dataSize);
};