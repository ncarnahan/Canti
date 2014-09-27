#ifndef GRAPHICS_PROGRAM_H
#define GRAPHICS_PROGRAM_H

#include "OpenGL.h"

namespace Graphics
{
    class Shader;

    //These are the indices for common shader attributes
    enum class ProgramAttribute : GLuint
    {
        Position,
        Normal,
        UV,
    };

    class Program
    {
    private:
        GLuint program;

    public:
        Program();
        ~Program();

        bool Load(const Shader& vertShader, const Shader& fragShader);
        bool Load(const Shader& vertShader, const Shader& geomShader, const Shader& fragShader);
        bool LoadFromFiles(const char* vertFile, const char* fragFile);
        bool LoadFromFiles(const char* vertFile, const char* geomFile, const char* fragFile);

        void Start();
        void Stop();

        inline GLint GetUniformLocation(const char* name) { return glGetUniformLocation(program, name); }

    private:
        bool Load(const Shader* vertShader, const Shader* geomShader, const Shader* fragShader);
    };
}

#endif
