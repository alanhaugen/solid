#ifndef NULLAUDIO_H
#define NULLAUDIO_H

#include <modules/audio/audio.h>

namespace Audio
{

class NullAudio : public IAudio
{
public:
    NullAudio();
    virtual ~NullAudio();
    virtual bool Init();
    virtual bool Shutdown();
    virtual void Stop();
    virtual void PlaySound(const char *sound, int type);
    virtual void Update();
};

}

#endif
