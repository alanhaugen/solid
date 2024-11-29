#include "portaudioaudio.h"

PortaudioAudio::PortaudioAudio()
{
    sample = NULL;
}

PortaudioAudio::~PortaudioAudio()
{
    err = Pa_StopStream(stream);

    if (err != paNoError)
    {
        LogError("Stream failed to stop");
    }

    err = Pa_CloseStream(stream);

    if (err != paNoError)
    {
        LogError("Stream failed to close");
    }

    delete[] sample;

    Pa_Terminate();
}

bool PortaudioAudio::Init()
{
    // Init Portaudio
    err = Pa_Initialize();

    if (err != paNoError)
    {
        LogError("Failed to initialise Portaudio");
        return false;
    }

    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;

    /*inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = data->sfInfo.channels;
    inputParameters.sampleFormat = paFloat32;//PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;//defaultLowOutputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;*/
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = channels;
    outputParameters.sampleFormat = paFloat32;//PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;//defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Stream
    err = Pa_OpenStream(
                &stream,
                NULL,//&inputParameters,
                &outputParameters,
                44100,//data->sfInfo.samplerate, // TODO: Check if this makes sense, or set back to data->sfInfo.samplerate
                paFramesPerBufferUnspecified,//BUFFER_LEN,//FRAMES_PER_BUFFER,
                paClipOff,      // we won't output out of range samples so don't bother clipping them
                NULL, // no callback, use blocking API
                NULL); // no callback, so no callback userData

    if (err != paNoError)
    {
        LogError("Failed to play audio");
        return false;
    }

    // Setup audioblock to play
    sample = new float[FRAMES_PER_BUFFER * channels];//data->sfInfo.channels]; //FRAMES_PER_BUFFER * data->sfInfo.channels * sizeof(float));

    if (sample == nullptr)
    {
        LogError("Failed to allocate memory for audio sample");

        return false;
    }

    // Start stream
    err = Pa_StartStream(stream);

    if (err != paNoError)
    {
        LogError("Failed to play audio");
        return false;
    }

    return true;
}

void PortaudioAudio::Stop()
{
    audioData.Clear();
}

void PortaudioAudio::PlaySound(const char *sound, int type)
{
    audioData.Add(new PortaudioData(sound, type));
}

/*
void PortaudioAudio::RecordSound()
{
    Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER); // Read from microphone
}*/

void PortaudioAudio::Update()
{
    int64_t readCount = 0;

    float volume = 0.0f;

    // Merge all audio together
    for (unsigned int i = 0; i < audioData.Size(); i++)
    {
        if (audioData[i]->type == Audio::SFX)
        {
            volume = audioVolume;
        }
        else
        {
            volume = audioVolumeMusic;
        }

        if (i == 0)
        {
            readCount = audioData[0]->Update(sample, volume);

            if (readCount == 0 && audioData.Size() > 1)
            {
                audioData.RemoveAt(0);
                readCount = audioData[0]->Update(sample, volume);
            }
        }
        else
        {
            audioData[i]->MixAudio(sample, volume);
        }
    }

    // Write audio to speakers
    err = Pa_WriteStream(stream, sample, readCount);

    // Check for errors
    if (err != paNoError)
    {
        if (err == PaErrorCode::paOutputUnderflowed)
        {
            LogWarning("Audio underflow. Not enough data.");
        }
        else
        {
            LogWarning("Error while playing audio stream: " + String(err));
        }
    }
}
