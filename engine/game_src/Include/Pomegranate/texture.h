#ifndef TEXTURE_H
#define TEXTURE_H

#include "resource_manager.h"
namespace Pomegranate
{
	struct Texture : Resource
	{
	private:
		SDL_Texture* texture;
		int width;
		int height;
	public:
		explicit Texture(const char* path);
		Texture(int width, int height);
		explicit Texture(SDL_Texture* tex);
		~Texture();
		[[nodiscard]] Vec2i get_size() const;
		void resize(int nwidth, int nheight);
		SDL_Texture* get_sdl_texture();
	};
}

#endif //TEXTURE_H
