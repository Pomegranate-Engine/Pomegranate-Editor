#ifndef AUDIO_H
#define AUDIO_H

#include "resource_manager.h"

namespace Pomegranate
{
	struct Audio : Resource
	{
	private:
		Mix_Chunk* audio;
	public:
		explicit Audio(const char* path);
		explicit Audio(Mix_Chunk* chunk);
		~Audio();
		Mix_Chunk* get_mix_chunk();
	};
}


#endif //AUDIO_H
