#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include<string>
#include "input_manager.h"
#include "debug.h"
/*----------> DEBUGGING <----------*/
namespace Pomegranate
{
	int pomegranate_init(bool input = true, bool video = true, bool audio = true, bool img = true, bool ttf = true);
	void pomegranate_quit();
	extern float delta_time;
}

#endif // ENGINE_H