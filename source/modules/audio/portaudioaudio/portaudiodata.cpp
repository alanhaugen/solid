#include "portaudiodata.h"
#include <core/x-platform/url.h>
// For fun later: https://zserge.com/posts/etude-in-c/

PortaudioData::PortaudioData(const char *filename, int type_)
{
    data = new AudioData;
    localSample = NULL;

    type = type_;

    // Try to open the file
    data->sndFile = sf_open(URL(filename), SFM_READ, &data->sfInfo);

    if (!data->sndFile)
    {
        LogError("error opening file " + String(filename));
    }
    else
    {
        // Setup local audio buffer
        localSample = new float[FRAMES_PER_BUFFER * data->sfInfo.channels];
    }
}

void PortaudioData::Seek()
{
    //sf_seek(data->sndFile, data->position, SEEK_SET);
}

int64_t PortaudioData::Update(float *sample, float audioVolume)
{
    // Read sound from data
    sf_count_t readCount = sf_readf_float(data->sndFile, sample, FRAMES_PER_BUFFER);

    // Adjust volume
    for (int channel = 0; channel < data->sfInfo.channels; channel++)
    {
        for (int i = channel; i < (readCount * data->sfInfo.channels); i += data->sfInfo.channels)
        {
             sample[i] *= audioVolume;
        }
    }

    return readCount;
}

//! See http://www.vttoth.com/CMS/index.php/technical-notes/68
void PortaudioData::MixAudio(float *sample, float audioVolume)
{
    int64_t readCount = Update(localSample, audioVolume);

    // Mix audio with main audio
    for (int channel = 0; channel < data->sfInfo.channels; channel++)
    {
        for (int i = channel; i < (readCount * data->sfInfo.channels); i += data->sfInfo.channels)
        {
            sample[i] = sample[i] + localSample[i] - ((sample[i]*localSample[i]) / (FLT_MAX)); // Z = A + B - AB/resolution
            sample[i] *= audioVolume;
        }
    }
}

PortaudioData::~PortaudioData()
{
    delete data;
    delete[] localSample;
}
