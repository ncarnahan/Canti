#include "Shader.h"
#include <iostream>
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
        program = 0;
    }

    Shader::~Shader()
    {
        if (program != 0)
        {
            glDeleteProgram(program);
        }
    }

    bool Shader::LoadFromFiles(std::string vertFile, std::string fragFile)
    {
        //Read source
        auto vertContents = File::ReadAllText(vertFile);
        auto fragContents = File::ReadAllText(fragFile);

        return LoadFromStrings(*vertContents, *fragContents);
    }

    bool Shader::LoadFromStrings(std::string& vertContents, std::string& fragContents)
    {
        program = glCreateProgram();
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
        //Load shaders
        const char* vertSource = vertContents.c_str();
        const char* fragSource = fragContents.c_str();
        glShaderSource(vertexShader, 1, &vertSource, NULL);
        glShaderSource(fragmentShader, 1, &fragSource, NULL);
 
        //Compile the shaders
        int success;
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            std::cout << "Vertex shader failed to compile.\n";
            std::cout << GetShaderCompileError(vertexShader);
            goto error;
        }

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            std::cout << "Fragment shader failed to compile.\n";
            std::cout << GetShaderCompileError(fragmentShader);
            goto error;
        }
 
        //Attach shaders to program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
 
        //Bind the attributes to a standard location
        glBindAttribLocation(program, (int)ShaderAttribute::Position, "in_position");
        glBindAttribLocation(program, (int)ShaderAttribute::Normal, "in_normal");
        glBindAttribLocation(program, (int)ShaderAttribute::UV, "in_uv");

        //Link the program
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            std::cout << "Program failed to link.\n";
            goto error;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return true;

error:
        //Delete the gl objects
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return false;
    }

    void Shader::Start()
    {
        glUseProgram(program);
    }

    void Shader::Stop()
    {
        glUseProgram(0);
    }
}
