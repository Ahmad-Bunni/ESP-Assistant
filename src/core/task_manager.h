#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

class TaskManager
{
public:
    static void initialize();
    static void startBackgroundTasks();
    static void triggerAudioProcessing();

private:
    static TaskHandle_t wifiMonitorTaskHandle;
    static TaskHandle_t audioTaskHandle;
    static TaskHandle_t aiTaskHandle;

    static QueueHandle_t recordQueue;
    static QueueHandle_t transcriptQueue;
    static SemaphoreHandle_t processingMutex;

    static void wifiMonitorTask(void *parameter);
    static void audioTask(void *parameter);
    static void aiTask(void *parameter);
};

#endif // TASK_MANAGER_H