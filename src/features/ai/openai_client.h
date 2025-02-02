#ifndef OPENAI_CLIENT_H
#define OPENAI_CLIENT_H

#include <Arduino.h>

class OpenAIClient
{
public:
    static String makeRequest(const String &userInput);

private:
    static String createRequestPayload(const String &userInput);
    static String extractMessageFromResponse(const String &response);
};

#endif // OPENAI_CLIENT_H