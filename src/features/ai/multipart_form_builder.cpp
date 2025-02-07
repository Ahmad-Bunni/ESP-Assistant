#include "features/audio/audio_conversion_service.h"
#include "openai_client.h"

MultipartFormBuilder::FormData MultipartFormBuilder::createAudioForm(const int16_t *audioBuffer, size_t bufferSize)
{
    FormData form;
    form.boundary = "AudioBoundary";
    form.contentType = "multipart/form-data; boundary=" + form.boundary;

    // Calculate form parts
    String head = "--" + form.boundary + "\r\n";
    head += "Content-Disposition: form-data; name=\"file\"; filename=\"audio.wav\"\r\n";
    head += "Content-Type: audio/wav\r\n\r\n";

    String tail = "\r\n--" + form.boundary + "\r\n";
    tail += "Content-Disposition: form-data; name=\"model\"\r\n\r\n";
    tail += "whisper-1\r\n";
    tail += "--" + form.boundary + "--\r\n";

    // Create WAV header
    uint8_t wavHeader[44];
    AudioConversionService::createWavHeader(wavHeader, bufferSize * 2);

    // Calculate total size and allocate memory
    form.totalLength = head.length() + 44 + (bufferSize * 2) + tail.length();
    form.payload = std::unique_ptr<uint8_t[]>(new uint8_t[form.totalLength]);

    // Build payload
    size_t pos = 0;
    memcpy(form.payload.get() + pos, head.c_str(), head.length());
    pos += head.length();
    memcpy(form.payload.get() + pos, wavHeader, 44);
    pos += 44;
    memcpy(form.payload.get() + pos, audioBuffer, bufferSize * 2);
    pos += bufferSize * 2;
    memcpy(form.payload.get() + pos, tail.c_str(), tail.length());

    return form;
}