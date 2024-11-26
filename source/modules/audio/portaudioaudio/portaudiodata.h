#ifndef PORTAUDIODATA_H
#define PORTAUDIODATA_H

#include <sndfile.h>
#include <core/x-platform/locator.h>

#define FRAMES_PER_BUFFER 1024

class PortaudioData
{
public:
    PortaudioData(const char *filename);
    ~PortaudioData();

    void Seek();
    void MixAudio(float *sample, float audioVolume);
    int64_t Update(float *sample, float audioVolume);

private:
    struct AudioData
    {
        SNDFILE *sndFile;
        SF_INFO sfInfo;
    };

    AudioData *data;
    float *localSample;
};

#endif // PORTAUDIODATA_H
