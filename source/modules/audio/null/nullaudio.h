#ifndef NULLAUDIO_H
#define NULLAUDIO_H

#include "../audio.h"

class NullAudio : public IAudio
{
public:
    NullAudio();
    virtual ~NullAudio();
    virtual bool Init();
    virtual bool Shutdown();
    virtual void Stop();
    virtual void PlaySound(const char *sound);
    virtual void Update();
};

#endif
