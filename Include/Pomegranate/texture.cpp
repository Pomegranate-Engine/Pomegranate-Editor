#include "texture.h"
namespace Pomegranate
{
    Texture::Texture(const char *path)
	{
		SDL_Surface* s = IMG_Load(path);
		SDL_Texture* t = SDL_CreateTextureFromSurface(Window::current->get_sdl_renderer(),s);
		texture = t;
		this->width = 0;
		this->height = 0;
		SDL_QueryTexture(texture, nullptr, nullptr,&width,&height);
		SDL_DestroySurface(s);
	}
    Texture::Texture(int width, int height)
	{
		this->width = width;
		this->height = height;
		this->texture = SDL_CreateTexture(Window::current->get_sdl_renderer(),SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,width,height);
	}

    Texture::Texture(SDL_Texture *tex)
	{
		texture = tex;
		this->width = 0;
		this->height = 0;
		SDL_QueryTexture(texture, nullptr, nullptr,&width,&height);
	}

    Texture::~Texture()
	{
		SDL_DestroyTexture(texture);
	}

	Vec2i Texture::get_size() const
	{
		return {width,height};
	}

	void Texture::resize(int nwidth, int nheight)
	{
		//TODO: implement when i have internet
	}

	SDL_Texture *Texture::get_sdl_texture()
	{
		return texture;
	}
}