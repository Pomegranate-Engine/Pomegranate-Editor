#include "ttf_font.h"
namespace Pomegranate
{
	TTFFont::TTFFont(TTF_Font *ttf)
	{
		this->font = ttf;
	}
	TTFFont::TTFFont(const char *path)
	{
		this->font = TTF_OpenFont(path,64);
	}
	TTFFont::~TTFFont()
	{
		TTF_CloseFont(this->font);
	}
	TTF_Font *TTFFont::get_ttf_font()
	{
		return this->font;
	}
}