#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

#include <Arduino.h>
#include <driver/i2s.h>

// Core configuration
#define SAMPLE_RATE 16000
#define RECORD_TIME 3000 // 5 seconds recording
#define BYTES_PER_SAMPLE 2
#define AUDIO_BUFFER_SIZE ((SAMPLE_RATE * RECORD_TIME / 1000) * BYTES_PER_SAMPLE)

// I2S pins
#define I2S_PORT I2S_NUM_0
#define I2S_SCK_PIN 9
#define I2S_WS_PIN 7
#define I2S_SD_PIN 8

class AudioService
{
public:
    static void setup();
    static void record();
    static const int16_t *getBuffer();
    static size_t getBufferSize();

private:
    static bool configureI2S();
    static int16_t *audioBuffer;
    static size_t audioBufferIndex;
    static const size_t CHUNK_SIZE = 1024;
    static int16_t tempBuffer[1024];
};

#endif // AUDIO_SERVICE_H