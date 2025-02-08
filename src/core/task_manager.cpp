#include "task_manager.h"
#include "core/wifi/wifi_manager.h"
#include "features/ai/openai_client.h"
#include "features/audio/audio_service.h"
#include <Arduino.h>

// Initialize static members
TaskHandle_t TaskManager::wifiMonitorTaskHandle = nullptr;
TaskHandle_t TaskManager::audioTaskHandle = nullptr;
TaskHandle_t TaskManager::aiTaskHandle = nullptr;

QueueHandle_t TaskManager::recordQueue = nullptr;
QueueHandle_t TaskManager::transcriptQueue = nullptr;
SemaphoreHandle_t TaskManager::processingMutex = nullptr;

void TaskManager::initialize()
{
    recordQueue = xQueueCreate(1, sizeof(bool));
    transcriptQueue = xQueueCreate(1, sizeof(bool));
    processingMutex = xSemaphoreCreateMutex();

    // Initialize services
    AudioService::setup();
    WiFiManager::connect();
}

void TaskManager::startBackgroundTasks()
{
    // Create background tasks
    xTaskCreatePinnedToCore(
        wifiMonitorTask,
        "WiFiMonitor",
        2048,
        nullptr,
        1,
        &wifiMonitorTaskHandle,
        0);

    xTaskCreatePinnedToCore(
        audioTask,
        "AudioProcessor",
        8192,
        nullptr,
        2,
        &audioTaskHandle,
        1);

    xTaskCreatePinnedToCore(
        aiTask,
        "AIProcessor",
        8192,
        nullptr,
        2,
        &aiTaskHandle,
        1);
}

void TaskManager::triggerAudioProcessing()
{
    bool trigger = true;
    xQueueSend(recordQueue, &trigger, portMAX_DELAY);
}

void TaskManager::wifiMonitorTask(void *parameter)
{
    while (true)
    {
        WiFiManager::isConnected();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void TaskManager::audioTask(void *parameter)
{
    bool trigger;
    while (true)
    {
        if (xQueueReceive(recordQueue, &trigger, portMAX_DELAY) == pdTRUE)
        {
            if (xSemaphoreTake(processingMutex, portMAX_DELAY) == pdTRUE)
            {
                AudioService::record();
                Serial.println("Finished recording");
                xQueueSend(transcriptQueue, &trigger, portMAX_DELAY);
                xSemaphoreGive(processingMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void TaskManager::aiTask(void *parameter)
{
    bool trigger;
    while (true)
    {
        if (xQueueReceive(transcriptQueue, &trigger, portMAX_DELAY) == pdTRUE)
        {
            if (xSemaphoreTake(processingMutex, portMAX_DELAY) == pdTRUE)
            {
                Serial.println("Starting transcription");
                String transcription = OpenAIClient::transcribeAudio(
                    AudioService::getBuffer(),
                    AudioService::getBufferSize());

                Serial.println("Making request");
                String response = OpenAIClient::makeRequest(transcription);
                Serial.println(response);
                xSemaphoreGive(processingMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}