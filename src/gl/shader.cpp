#include "shader.h"

Shader::Shader(const char* path)
{
    //Load shader from file
    std::ifstream file(path);
    if (!file.is_open())
    {
        print_error("Failed to open file: %s", path);
    }
    std::string vertex_shader_s;
    std::string fragment_shader_s;
    std::string line;
    bool vertex = false;
    bool fragment = false;
    while (std::getline(file, line))
    {
        if (line == "#vertex")
        {
            vertex = true;
            fragment = false;
            continue;
        }
        if (line == "#fragment")
        {
            vertex = false;
            fragment = true;
            continue;
        }
        if (vertex)
        {
            vertex_shader_s += line + "\n";
        }
        if (fragment)
        {
            fragment_shader_s += line + "\n";
        }
    }
    file.close();

    const char* vertex_shader = vertex_shader_s.c_str();
    const char* fragment_shader = fragment_shader_s.c_str();
    //Compile vertex shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
    glCompileShader(vertex_shader_id);
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        print_error("Vertex shader compilation failed: %s", info_log);
    }

    //Compile fragment shader
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
    glCompileShader(fragment_shader_id);
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        print_error("Fragment shader compilation failed: %s", info_log);
    }

    //Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader_id);
    glAttachShader(ID, fragment_shader_id);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        print_error("Shader linking failed: %s", info_log);
    }

    //Delete shaders
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}
