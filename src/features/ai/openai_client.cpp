#include "openai_client.h"
#include "core/wifi/wifi_manager.h"
#include "features/led/led_manager.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

String OpenAIClient::makeRequest(const String &userInput)
{
    if (!WiFiManager::isConnected())
    {
        return "Error: WiFi not connected";
    }

    LEDManager::setRequestInProgress(true);

    HTTPClient http;
    http.setTimeout(10000);
    http.begin(OPENAI_ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

    String jsonPayload = createRequestPayload(userInput);

    int httpResponseCode = http.POST(jsonPayload);

    String response = "Error making request";

    if (httpResponseCode > 0)
    {
        String rawResponse = http.getString();
        response = extractMessageFromResponse(rawResponse);
    }
    else
    {
        Serial.println("Error on HTTP request");
    }

    http.end();
    LEDManager::setRequestInProgress(false);
    return response;
}

String OpenAIClient::createRequestPayload(const String &userInput)
{
    StaticJsonDocument<1024> doc;
    doc["model"] = OPENAI_MODEL;
    doc["temperature"] = 0.7;

    JsonArray messages = doc.createNestedArray("messages");
    JsonObject userMessage = messages.createNestedObject();
    userMessage["role"] = "user";
    userMessage["content"] = userInput;

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

String OpenAIClient::extractMessageFromResponse(const String &response)
{
    StaticJsonDocument<4096> responseDoc;
    DeserializationError error = deserializeJson(responseDoc, response);

    if (!error)
    {
        return responseDoc["choices"][0]["message"]["content"].as<String>();
    }
    return "Error parsing response";
}