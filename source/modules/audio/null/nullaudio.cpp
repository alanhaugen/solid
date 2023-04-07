#include "nullaudio.h"

NullAudio::NullAudio() {
}

NullAudio::~NullAudio() {
}

bool NullAudio::Init() {
  return true;
}

bool NullAudio::Shutdown() {
  return true;
}

void NullAudio::Stop()
{
}

void NullAudio::PlaySound(const char* sound) {
    (void)(sound);
}

void NullAudio::Update() {
}
