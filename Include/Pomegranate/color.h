#ifndef POMEGRANATE_ENGINE_COLOR_H
#define POMEGRANATE_ENGINE_COLOR_H
#include <cmath>
namespace Pomegranate
{
    class Color
    {
    public:
        int r,g,b,a;
        Color();
        Color(int r, int g, int b, int a=255);
        Color(float hue, float brightness, float a=1);
        explicit Color(int hex);
        Color(float r, float g, float b, float a=1);
        Color operator+(const Color& other) const;
        Color operator-(const Color& other) const;
        Color operator*(float other) const;
        Color operator/(float other) const;
        [[nodiscard]] Color mix(const Color& other, float amount) const;
    };
}

#endif //POMEGRANATE_ENGINE_COLOR_H
