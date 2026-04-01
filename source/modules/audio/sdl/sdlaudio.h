#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include <modules/audio/audio.h>
#include <modules/audio/null/nullaudio.h>
#include <SDL_audio.h>

namespace Audio
{

class SDLAudio : public NullAudio
{
private:
    SDL_AudioDeviceID device;

public:
    SDLAudio();
    virtual ~SDLAudio();
    virtual bool Init();
    virtual bool Shutdown();
    virtual void Stop();
    virtual void PlaySound(const char *sound, int type);
};

}

#endif
