#include "nullaudio.h"

Audio::NullAudio::NullAudio()
{
}

Audio::NullAudio::~NullAudio()
{
}

bool Audio::NullAudio::Init()
{
  return true;
}

bool Audio::NullAudio::Shutdown()
{
  return true;
}

void Audio::NullAudio::Stop()
{
}

void Audio::NullAudio::PlaySound(const char* sound, int type = Audio::SFX)
{
    (void)(sound);
    (void)(type);
}

void Audio::NullAudio::Update()
{
}
