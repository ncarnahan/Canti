#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include "Texture.h"
#include <vector>

namespace Graphics
{
    class Program;

    class Material
    {
    private:
        Program* _program;

        //The first item of the pair is the shader uniform location
        std::vector<std::pair<GLint, Texture&>> _textures;

    public:
        Material();

        void Start();

        inline void SetShader(Program& program) { _program = &program; }
        void SetTexture(GLint location, Texture& texture);
    };
}

#endif
