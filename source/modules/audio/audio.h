#ifndef AUDIO_H
#define AUDIO_H

namespace Audio
{

class IAudio {
public:
    virtual ~IAudio() {}
    virtual bool Init() = 0;
    virtual void Stop() = 0;
    virtual void PlaySound(const char *sound) = 0;
    virtual void Update() = 0;
};

}

#endif
