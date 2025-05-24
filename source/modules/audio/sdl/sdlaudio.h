#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include <modules/audio/audio.h>
#include <SDL_audio.h>

namespace Audio
{

class SDLAudio : public IAudio
{
private:
    SDL_AudioStream *stream;

public:
    SDLAudio();
    virtual ~SDLAudio();
    virtual bool Init();
    virtual bool Shutdown();
    virtual void Stop();
    virtual void PlaySound(const char *sound, int type);
    virtual void Update();
};

}

#endif
