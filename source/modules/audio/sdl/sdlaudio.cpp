#include "sdlaudio.h"
#include <SDL.h>
#include <iostream>

Audio::SDLAudio::SDLAudio()
{
    device = 0;
}

Audio::SDLAudio::~SDLAudio()
{
    if (device)
        SDL_CloseAudioDevice(device);
}

bool Audio::SDLAudio::Init()
{
    SDL_AudioSpec wanted;
    SDL_zero(wanted);
    wanted.freq = 44100;
    wanted.format = AUDIO_F32SYS;
    wanted.channels = 2;
    wanted.samples = 4096;
    wanted.callback = NULL;
    wanted.userdata = NULL;

    SDL_AudioSpec obtained;
    device = SDL_OpenAudioDevice(NULL, 0, &wanted, &obtained, 0);
    if (device == 0)
    {
        SDL_Log("SDLAudio Init error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool Audio::SDLAudio::Shutdown()
{
    if (device)
    {
        SDL_CloseAudioDevice(device);
        device = 0;
    }
    return true;
}

void Audio::SDLAudio::Stop()
{
    if (device)
        SDL_ClearQueuedAudio(device);
}

void Audio::SDLAudio::PlaySound(const char *sound, int type)
{
    (void)type;
    if (!device)
        return;

    SDL_AudioSpec spec;
    Uint8 *wav_buffer = NULL;
    Uint32 wav_length = 0;
    if (SDL_LoadWAV(sound, &spec, &wav_buffer, &wav_length) == NULL)
    {
        SDL_Log("Failed to load WAV %s: %s", sound, SDL_GetError());
        return;
    }
    if (spec.format != AUDIO_F32SYS) {
        SDL_Log("Unsupported audio format for %s", sound);
    } else {
        if (SDL_QueueAudio(device, wav_buffer, wav_length) < 0)
        {
            SDL_Log("Failed to queue audio: %s", SDL_GetError());
        }
    }

    SDL_FreeWAV(wav_buffer);
    SDL_PauseAudioDevice(device, 0); // start playback
}
