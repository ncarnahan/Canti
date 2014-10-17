#include "Material.h"
#include "Program.h"

namespace Graphics
{
    Material::Material()
    {
    }

    void Material::Start()
    {
        _program->Start();

        //Bind textures
        for (size_t i = 0; i < _textures.size(); i++)
        {
            auto& property = _textures[i];

            glUniform1i(property.uniform, i);
            glActiveTexture(GL_TEXTURE0 + i);
            property.texture->Bind();
        }

        for (auto& thing : _floatStorage)
        {
            glUniform1f(thing.uniform, thing.value);
        }

        for (auto& thing : _vector3Storage)
        {
            glUniform3fv(thing.uniform, 1, &thing.value[0]);
        }
    }

    void Material::SetProgram(Program& program)
    {
        _program = &program;

        //Clear material properties
        _textures.clear();
    }

    void Material::SetTexture(GLint location, Texture& texture)
    {
        //Overwrite it if it already exists
        for (auto& property : _textures)
        {
            if (property.uniform == location)
            {
                property.texture = &texture;
                return;
            }
        }

        //It doesn't exist; add it.
        PropTexture property;
        property.uniform = location;
        property.texture = &texture;
        _textures.push_back(property);
    }

    void Material::SetFloat(GLint location, float value)
    {
        //Overwrite it if it already exists
        for (auto& property : _floatStorage)
        {
            if (property.uniform == location)
            {
                property.value = value;
                return;
            }
        }

        //It doesn't exist; add it.
        PropFloat property;
        property.uniform = location;
        property.value = value;
        _floatStorage.push_back(property);
    }

    void Material::SetVector3(GLint location, Vector3 value)
    {
        //Overwrite it if it already exists
        for (auto& property : _vector3Storage)
        {
            if (property.uniform == location)
            {
                property.value = value;
                return;
            }
        }

        //It doesn't exist; add it.
        PropVector3 property;
        property.uniform = location;
        property.value = value;
        _vector3Storage.push_back(property);
    }
}
