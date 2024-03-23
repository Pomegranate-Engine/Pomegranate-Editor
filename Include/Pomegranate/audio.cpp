#include "audio.h"

namespace Pomegranate
{
    Audio::Audio(Mix_Chunk *chunk)
	{
		this->audio = chunk;
	}
	Audio::Audio(const char *path)
	{
		this->audio = Mix_LoadWAV(path);
	}
	Audio::~Audio()
	{
		Mix_FreeChunk(audio);
	}
	Mix_Chunk* Audio::get_mix_chunk()
	{
		return audio;
	}
}