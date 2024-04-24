template <typename T>
void Shader::set(const char *name, T value) const
{
    GLint location = glGetUniformLocation(ID, name);
    if (location == -1) {
        std::cerr << "Uniform " << name << " not found in shader program!" << std::endl;
        return;
    }

    if(typeid(T) == typeid(Vec4))
    {
        Vec4 v = *reinterpret_cast<Vec4*>(&value);
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }
    else if(typeid(T) == typeid(Vec3))
    {
        Vec3 v = *reinterpret_cast<Vec3*>(&value);
        glUniform3f(location, v.x, v.y, v.z);
    }
    else if(typeid(T) == typeid(Vec2))
    {
        Vec2 v = *reinterpret_cast<Vec2*>(&value);
        glUniform2f(location, v.x, v.y);
    }
    else if(typeid(T) == typeid(Mat))
    {
        //Determining the size of the matrix
        int cols = ((Mat)value).cols;
        int rows = ((Mat)value).rows;

        //Set the matrix
        float* data = new float[cols * rows];
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                data[i * rows + j] = ((Mat)value).data[j][i];
            }
        }
        if (cols == 2 && rows == 2)
        {
            glUniformMatrix2fv(location, 1, GL_FALSE, data);
        }
        else if (cols == 3 && rows == 3)
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, data);
        }
        else if (cols == 4 && rows == 4)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, data);
        }
        else
        {
            std::cerr << "Unsupported matrix size!" << std::endl;
        }
    }
    else if(typeid(T) == typeid(float))
    {
        glUniform1f(location, *reinterpret_cast<float*>(&value));
    }
    else if(typeid(T) == typeid(int))
    {
        glUniform1i(location, *reinterpret_cast<int*>(&value));
    }
    else if(typeid(T) == typeid(Texture*))
    {
        SDL_Texture* sdlTexture = (*reinterpret_cast<Texture**>(&value))->get_sdl_texture();
        SDL_GL_BindTexture(sdlTexture, NULL, NULL);
        glUniform1i(location, 0);
    }

    else
    {
        std::cerr << "Unsupported uniform type!" << std::endl;
    }
}
