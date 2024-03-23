#include "color.h"
namespace Pomegranate
{
    Color::Color()
    {
        this->r = 255;
        this->g = 255;
        this->b = 255;
        this->a = 255;
    }

    Color::Color(int r, int g, int b, int a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color::Color(int hex)
    {
        this->r = (int)(hex & 0xFF000000) >> 24;
        this->g = (int)(hex & 0x00FF0000) >> 16;
        this->b = (int)(hex & 0x0000FF00) >> 8;
        this->a = (int)(hex & 0x000000FF);
    }

    Color::Color(float r, float g, float b, float a)
    {
        this->r = (int)(r*255);
        this->g = (int)(g*255);
        this->b = (int)(b*255);
        this->a = (int)(a*255);
    }

    Color Color::operator+(const Color &other) const
    {
        return {this->r+other.r, this->g+other.g, this->b+other.b, this->a+other.a};
    }

    Color Color::operator-(const Color &other) const
    {
        return {this->r-other.r, this->g-other.g, this->b-other.b, this->a-other.a};
    }

    Color Color::operator*(float other) const
    {
        return {((float)this->r/255.0f)*other, ((float)this->g/255.0f)*other, ((float)this->b/255.0f)*other, ((float)this->a/255.0f)*other};
    }

    Color Color::operator/(float other) const
    {
        return {((float)this->r/255.0f)/other, ((float)this->g/255.0f)/other, ((float)this->b/255.0f)/other, ((float)this->a/255.0f)/other};
    }

    Color Color::mix(const Color &other, float amount) const
    {
        return *this * (1.0f - amount) + other * amount;
    }

    Color::Color(float hue, float brightness, float a)
    {
        float c = brightness;
        float x = c * (1.0f - fabsf(fmod(hue / 60.0f, 2.0f) - 1.0f));
        float m = brightness - c;
        float _r = 0.0f;
        float _g = 0.0f;
        float _b = 0.0f;
        if (hue >= 0.0f && hue < 60.0f)
        {
            _r = (c + m) * 255.0f;
            _g = (x + m) * 255.0f;
            _b = m * 255.0f;
        }
        else if (hue >= 60.0f && hue < 120.0f)
        {
            _r = (x + m) * 255.0f;
            _g = (c + m) * 255.0f;
            _b = m * 255.0f;
        }
        else if (hue >= 120.0f && hue < 180.0f)
        {
            _r = m * 255.0f;
            _g = (c + m) * 255.0f;
            _b = (x + m) * 255.0f;
        }
        else if (hue >= 180.0f && hue < 240.0f)
        {
            _r = m * 255.0f;
            _g = (x + m) * 255.0f;
            _b = (c + m) * 255.0f;
        }
        else if (hue >= 240.0f && hue < 300.0f)
        {
            _r = (x + m) * 255.0f;
            _g = m * 255.0f;
            _b = (c + m) * 255.0f;
        }
        else if (hue >= 300.0f && hue < 360.0f)
        {
            _r = (c + m) * 255.0f;
            _g = m * 255.0f;
            _b = (x + m) * 255.0f;
        }
        this->r = (int)_r;
        this->g = (int)_g;
        this->b = (int)_b;
        this->a = (int)(a * 255);
    }
}
