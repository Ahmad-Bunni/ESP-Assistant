#include "core/task_manager.h"
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  TaskManager::initialize();
  TaskManager::startBackgroundTasks();
}

void loop()
{
  if (Serial.available())
  {
    while (Serial.available())
      Serial.read();

    Serial.println("Triggering audio processing");
    TaskManager::triggerAudioProcessing();
  }

  vTaskDelay(pdMS_TO_TICKS(10));
}
