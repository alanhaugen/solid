#ifndef AUDIO_H
#define AUDIO_H

namespace Audio
{

enum
{
    SFX,
    MUSIC
};

class IAudio {
public:
    virtual ~IAudio() {}
    virtual bool Init() = 0;
    virtual void Stop() = 0;
    virtual void PlaySound(const char *sound, int type = SFX) = 0;
    virtual void Update() = 0;

    float audioVolume = 1.0f;
    float audioVolumeMusic = 1.0f;
};

}

#endif
