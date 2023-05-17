#ifndef PORTAUDIOAUDIO_H
#define PORTAUDIOAUDIO_H

#include "../audio.h"
#include "portaudiodata.h"
#include "core/x-platform/locator.h"
#include "core/containers/array.h"
#include <fstream>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>

const int channels = 2;//data->sfInfo.channels; // TODO: Check if this makes sense, or set back to data->sfInfo.channels somehow

class PortaudioAudio : public Audio::IAudio
{
public:
    PortaudioAudio();
    virtual ~PortaudioAudio();
    virtual bool Init();
    virtual void Stop();
    virtual void PlaySound(const char *sound);
    virtual void Update();

private:
    float *sample;

    Array<PortaudioData*> audioData;

    PaError err;
    PaStream *stream;
};

#endif
