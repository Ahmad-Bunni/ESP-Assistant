#include "audio_conversion_service.h"
#include "audio_service.h"

void AudioConversionService::createWavHeader(uint8_t *header, int dataSize)
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