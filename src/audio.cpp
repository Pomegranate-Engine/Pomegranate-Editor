#include "audio.h"

int AudioManager::sample_rate = 48000;

int AudioManager::patestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    float frequency = 440;
    float M_PI = 3.14159265358979323846;
    //Generate sine wave
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = (float) sin(data->left_phase);
        *out++ = (float) sin(data->right_phase);
        data->left_phase += 2.0f * M_PI * frequency / sample_rate;
        if( data->left_phase > 2.0f * M_PI ) data->left_phase -= 2.0f * M_PI;
        data->right_phase += 2.0f * M_PI * frequency / sample_rate;
        if( data->right_phase > 2.0f * M_PI ) data->right_phase -= 2.0f * M_PI;
    }
    return 0;
}

void AudioManager::init(int sample_rate, int frames_per_buffer)
{
    AudioManager::sample_rate = sample_rate;
    error(Pa_Initialize());

    static paTestData data;
    PaStream *stream;
    /* Open an audio I/O stream. */
    error(Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, sample_rate, frames_per_buffer, patestCallback, &data));
    error(Pa_StartStream(stream));
}

int AudioManager::error(PaError err)
{
    if(err != paNoError)
    {
        print_error("PortAudio error: " + std::string(Pa_GetErrorText(err)));
    }
    return err;
}
