#include "nullaudio.h"

Audio::SDLAudio::SDLAudio()
{
}

Audio::SDLAudio::~SDLAudio()
{
}

bool Audio::SDLAudio::Init()
{
  return true;
}

bool Audio::SDLAudio::Shutdown()
{
  return true;
}

void Audio::SDLAudio::Stop()
{
}

void Audio::SDLAudio::PlaySound(const char* sound, int type = Audio::SFX)
{
    (void)(sound);
    (void)(type);
}

void Audio::SDLAudio::Update()
{
}
