#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "OpenGL.h"
#include <string>

namespace Graphics
{
    //These are the indices for common shader attributes
    enum class ShaderAttribute : GLint
    {
        Position,
        Normal,
        UV,
    };

    class Shader
    {
    private:
        GLuint program;

    public:
        Shader();
        ~Shader();

        bool LoadFromFiles(std::string vertFile, std::string fragFile);
        bool LoadFromStrings(std::string& vertContents, std::string& fragContents);

        void Start();
        void Stop();

        inline GLuint GetProgram() { return program; }
    };
}

#endif
