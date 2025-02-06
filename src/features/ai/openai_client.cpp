#include "openai_client.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

String OpenAIClient::makeRequest(const String &userInput)
{
    HTTPClient http;
    http.setTimeout(10000);
    http.begin(OPENAI_ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

    // Create request payload
    JsonDocument doc;
    doc["model"] = OPENAI_MODEL;
    doc["temperature"] = 0.7;
    JsonArray messages = doc["messages"].to<JsonArray>();
    JsonObject userMessage = messages.add<JsonObject>();
    userMessage["role"] = "user";
    userMessage["content"] = userInput;

    String jsonPayload;
    serializeJson(doc, jsonPayload);

    String response = "Error";
    int httpCode = http.POST(jsonPayload);
    if (httpCode > 0)
    {
        JsonDocument responseDoc;
        if (!deserializeJson(responseDoc, http.getString()))
        {
            response = responseDoc["choices"][0]["message"]["content"].as<String>();
        }
    }

    http.end();
    return response;
}

String OpenAIClient::transcribeAudio(const int16_t *audioBuffer, size_t bufferSize)
{
    HTTPClient http;
    http.setTimeout(30000);
    http.begin("https://api.openai.com/v1/audio/transcriptions");
    http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

    // Create WAV header
    const int headerSize = 44;
    uint8_t header[headerSize];
    createWavHeader(header, bufferSize * 2);

    // Prepare multipart form data
    String boundary = "AudioBoundary";
    http.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

    String head = "--" + boundary + "\r\n";
    head += "Content-Disposition: form-data; name=\"file\"; filename=\"audio.wav\"\r\n";
    head += "Content-Type: audio/wav\r\n\r\n";

    String tail = "\r\n--" + boundary + "\r\n";
    tail += "Content-Disposition: form-data; name=\"model\"\r\n\r\n";
    tail += "whisper-1\r\n";
    tail += "--" + boundary + "--\r\n";

    size_t totalLength = head.length() + headerSize + (bufferSize * 2) + tail.length();
    http.addHeader("Content-Length", String(totalLength));

    uint8_t *payload = (uint8_t *)malloc(totalLength);
    if (!payload)
        return "Memory error";

    // Build payload
    size_t pos = 0;
    memcpy(payload + pos, head.c_str(), head.length());
    pos += head.length();
    memcpy(payload + pos, header, headerSize);
    pos += headerSize;
    memcpy(payload + pos, audioBuffer, bufferSize * 2);
    pos += bufferSize * 2;
    memcpy(payload + pos, tail.c_str(), tail.length());

    String response = "Error";
    int httpCode = http.POST(payload, totalLength);
    if (httpCode > 0)
    {
        JsonDocument doc;
        if (!deserializeJson(doc, http.getString()))
        {
            response = doc["text"].as<String>();
        }
    }

    free(payload);
    http.end();
    return response;
}

void OpenAIClient::createWavHeader(uint8_t *header, int dataSize)
{
    // RIFF chunk
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    *(uint32_t *)(header + 4) = 36 + dataSize;
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';

    // fmt chunk
    header[12] = 'f';
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    *(uint32_t *)(header + 16) = 16;
    *(uint16_t *)(header + 20) = 1;               // PCM
    *(uint16_t *)(header + 22) = 1;               // Mono
    *(uint32_t *)(header + 24) = SAMPLE_RATE;     // Sample rate
    *(uint32_t *)(header + 28) = SAMPLE_RATE * 2; // Byte rate
    *(uint16_t *)(header + 32) = 2;               // Block align
    *(uint16_t *)(header + 34) = 16;              // Bits per sample

    // data chunk
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    *(uint32_t *)(header + 40) = dataSize;
}