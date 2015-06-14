#include "Shader.h"
#include <iostream>
#include <string>
#include <Utils/File.h>
using namespace Utils;

namespace Graphics
{
    std::string GetShaderCompileError(GLuint shader)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length + 1];
        glGetShaderInfoLog(shader, length, NULL, buffer);

        std::string output(buffer, length + 1);

        delete buffer;

        return output;
    }

    Shader::Shader()
    {
        shader = 0;
    }

    Shader::~Shader()
    {
        if (shader != 0)
        {
            glDeleteShader(shader);
        }
    }

    bool Shader::LoadFromFile(ShaderType type, const char* fileName)
    {
        //Read source
        std::string contents = File::ReadAllText(fileName);

        return LoadFromString(type, contents.c_str());
    }

    bool Shader::LoadFromString(ShaderType type, const char* source)
    {
        //Create
        shader = glCreateShader((GLuint)type);

        //Load source
        glShaderSource(shader, 1, &source, NULL);

        //Compile
        int success;
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            std::cout << "Shader failed to compile.\n";
            std::cout << GetShaderCompileError(shader);
            return false;
        }

        return true;
    }
}
