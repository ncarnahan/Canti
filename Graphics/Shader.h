#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "OpenGL.h"
#include <string>

namespace Graphics
{
    enum class ShaderType : GLuint
    {
        Vertex = GL_VERTEX_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
    };

    class Shader
    {
    private:
        GLuint shader;

    public:
        Shader();
        ~Shader();

        bool LoadFromFile(ShaderType type, const char* fileName);
        bool LoadFromString(ShaderType type, const char* source);

        inline GLuint GetShader() const { return shader; }
    };
}

#endif
