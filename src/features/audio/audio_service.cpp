#include "audio_service.h"

int16_t *AudioService::audioBuffer = nullptr;
size_t AudioService::audioBufferIndex = 0;
int16_t AudioService::tempBuffer[1024];

void AudioService::setup()
{
    // Allocate buffer
    audioBuffer = (int16_t *)malloc(AUDIO_BUFFER_SIZE);
    if (!audioBuffer)
    {
        Serial.println("Failed to allocate audio buffer");
    }

    configureI2S();
}

bool AudioService::configureI2S()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0};

    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_SCK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD_PIN};

    if (i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL) != ESP_OK)
    {
        return false;
    }

    if (i2s_set_pin(I2S_PORT, &pin_config) != ESP_OK)
    {
        i2s_driver_uninstall(I2S_PORT);
        return false;
    }

    return true;
}

void AudioService::record()
{
    if (!audioBuffer)
        return;

    audioBufferIndex = 0;
    unsigned long startTime = millis();

    // Record for 5 seconds
    while ((millis() - startTime) < RECORD_TIME)
    {
        size_t bytesIn = 0;
        if (i2s_read(I2S_PORT, &tempBuffer, CHUNK_SIZE, &bytesIn, portMAX_DELAY) == ESP_OK)
        {
            size_t samples = bytesIn / 2;
            if (audioBufferIndex + samples <= (AUDIO_BUFFER_SIZE / 2))
            {
                memcpy(&audioBuffer[audioBufferIndex], tempBuffer, bytesIn);
                audioBufferIndex += samples;
            }
        }
        yield();
    }
}

const int16_t *AudioService::getBuffer()
{
    return audioBuffer;
}

size_t AudioService::getBufferSize()
{
    return audioBufferIndex;
}