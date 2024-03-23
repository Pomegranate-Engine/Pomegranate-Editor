#ifndef TTFFONT_H
#define TTFFONT_H

#include "resource_manager.h"
namespace Pomegranate
{
	struct TTFFont : Resource
	{
	private:
		TTF_Font* font;
	public:
		TTFFont(const char* path);
		TTFFont(TTF_Font* ttf);
		~TTFFont();
		TTF_Font* get_ttf_font();
	};
}

#endif //TTFFONT_H
