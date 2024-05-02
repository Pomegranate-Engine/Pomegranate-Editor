#ifndef POMEGRANATE_ENGINE_AUDIO_H
#define POMEGRANATE_ENGINE_AUDIO_H
#include<portaudio/portaudio.h>
#include<cmath>
#include<Pomegranate/engine.h>

using namespace Pomegranate;

struct paTestData
{
    float left_phase;
    float right_phase;
};
class AudioManager
{
private:
    static int sample_rate;
public:
    static int patestCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );
    static int error(PaError err);
    static void init(int sample_rate, int frames_per_buffer);
};


#endif //POMEGRANATE_ENGINE_AUDIO_H
