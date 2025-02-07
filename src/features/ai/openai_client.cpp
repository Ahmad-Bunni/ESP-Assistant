#include "openai_client.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

String OpenAIClient::makeRequest(const String &userInput)
{
    HTTPClient http;
    http.setTimeout(10000);
    http.begin(String(OPENAI_ENDPOINT) + "chat/completions");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

    String jsonPayload = createRequestPayload(userInput);
    int httpCode = http.POST(jsonPayload);
    String response = handleApiResponse(httpCode, http.getString());

    http.end();
    return response;
}

String OpenAIClient::transcribeAudio(const int16_t *audioBuffer, size_t bufferSize)
{
    if (!audioBuffer || bufferSize == 0)
    {
        return "Invalid audio data";
    }

    HTTPClient http;
    http.setTimeout(30000);
    http.begin(String(OPENAI_ENDPOINT) + "audio/transcriptions");
    http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

    auto formData = MultipartFormBuilder::createAudioForm(audioBuffer, bufferSize);
    http.addHeader("Content-Type", formData.contentType);
    http.addHeader("Content-Length", String(formData.totalLength));

    int httpCode = http.POST(formData.payload.get(), formData.totalLength);
    String response = handleApiResponse(httpCode, http.getString());

    http.end();
    return response;
}

String OpenAIClient::handleApiResponse(int httpCode, const String &response)
{
    if (httpCode <= 0)
    {
        return "HTTP Error: " + String(httpCode);
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error)
    {
        return "JSON parsing error: " + String(error.c_str());
    }

    if (doc["error"].is<JsonObject>())
    {
        return "API Error: " + doc["error"]["message"].as<String>();
    }

    // Handle chat completion response
    if (doc["choices"].is<JsonArray>())
    {
        return doc["choices"][0]["message"]["content"].as<String>();
    }

    // Handle transcription response
    if (doc["text"].is<const char *>())
    {
        return doc["text"].as<String>();
    }

    return "Unknown response format";
}

String OpenAIClient::createRequestPayload(const String &userInput)
{
    JsonDocument doc;
    doc["model"] = OPENAI_MODEL;
    doc["temperature"] = 0.7;

    JsonArray messages = doc["messages"].to<JsonArray>();
    JsonObject userMessage = messages.add<JsonObject>();
    userMessage["role"] = "user";
    userMessage["content"] = userInput;

    String jsonPayload;
    serializeJson(doc, jsonPayload);
    return jsonPayload;
}
