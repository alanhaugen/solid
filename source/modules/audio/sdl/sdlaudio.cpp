#include "nullaudio.h"
#include <application.h>

Audio::SDLAudio::SDLAudio()
{
}

Audio::SDLAudio::~SDLAudio()
{
}

bool Audio::SDLAudio::Init()
{
    stream = SDL_NewAudioStream(AUDIO_S16, 1, 22050, AUDIO_F32, 2, 48000);

    if (stream == NULL)
    {
        Log("Uhoh, stream failed to create: " + SDL_GetError());

        return false;
    }

    return true;
}

bool Audio::SDLAudio::Shutdown()
{
    SDL_FreeAudioStream(stream);

    return true;
}

void Audio::SDLAudio::Stop()
{
    SDL_AudioStreamClear(stream);
}

void Audio::SDLAudio::PlaySound(const char* sound, int type = Audio::SFX)
{
    (void)(sound);
    (void)(type);

    Sint16 samples[1024];
    //int num_samples = read_more_samples_from_disk(samples); // whatever.
    // you tell it the number of _bytes_, not samples, you're putting!

    int rc = SDL_AudioStreamPut(stream, samples, num_samples * sizeof (Sint16));
    if (rc == -1)
    {
        Log("Uhoh, failed to put samples in stream: " + SDL_GetError());
        return;
    }
}

void Audio::SDLAudio::Update()
{
    SDL_AudioStreamFlush(stream); // remove?
}
