#ifndef AUDIO_CONVERSION_SERVICE_H
#define AUDIO_CONVERSION_SERVICE_H

#include <cstddef>
#include <cstdint>

class AudioConversionService
{
public:
    static void createWavHeader(uint8_t *header, int dataSize);
};

#endif // AUDIO_CONVERSION_SERVICE_H