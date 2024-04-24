#ifndef POMEGRANATE_ENGINE_SHADER_H
#define POMEGRANATE_ENGINE_SHADER_H
#include<SDL3/SDL.h>
#include<glew/glew.h>
#include<GL/GL.h>
#include<GL/GLU.h>

#include<Pomegranate/engine.h>
#include<Pomegranate/vec_math.h>
#include<Pomegranate/resource_manager.h>
#include<Pomegranate/texture.h>
#include<fstream>
using namespace Pomegranate;

class Shader : Resource
{
public:
    unsigned int ID;
    Shader(const char* path);
    ~Shader();
    void use();

    template<typename T> void set(const char* name, T value) const;
};

#include "shader.inl"
#endif //POMEGRANATE_ENGINE_SHADER_H
