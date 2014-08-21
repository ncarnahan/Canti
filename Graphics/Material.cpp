#include "Material.h"

namespace Graphics
{
    Material::Material()
    {
    }

    void Material::Start()
    {
        _shader->Start();

        for (size_t i = 0; i < _textures.size(); i++)
        {
            auto& pair = _textures[i];

            glUniform1i(pair.first, i);
            glActiveTexture(GL_TEXTURE0 + i);
            pair.second.Bind();
            //glBindSampler(i, SamplerHandle);
        }
    }

    void Material::SetTexture(GLint location, Texture& texture)
    {
        //Overwrite it if it already exists
        for (auto& pair : _textures)
        {
            if (pair.first == location)
            {
                pair.second = texture;
                return;
            }
        }

        //It doesn't exist; add it.
        _textures.push_back(std::pair<GLuint, Texture&>(location, texture));
    }
}
