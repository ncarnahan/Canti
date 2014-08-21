#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include <vector>

namespace Graphics
{
    class Material
    {
    private:
        Shader* _shader;

        //The first item of the pair is the shader uniform location
        std::vector<std::pair<GLint, Texture&>> _textures;

    public:
        Material();

        void Start();

        inline void SetShader(Shader& shader) { _shader = &shader; }
        void SetTexture(GLint location, Texture& texture);
    };
}

#endif
