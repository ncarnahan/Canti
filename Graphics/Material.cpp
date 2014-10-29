#include "Material.h"
#include "Program.h"

namespace Graphics
{
    void FirstPass(BlendType blendType){
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        //Set up blending
        if (blendType == BlendType::Opaque || blendType == BlendType::AlphaTested)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            if (blendType == BlendType::Additive)
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }
            else
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
        }
    }

    void SecondPass(BlendType blendType)
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);

        if (blendType == BlendType::Opaque || blendType == BlendType::AlphaTested)
        {
            glBlendFunc(GL_ONE, GL_ONE);
        }
        else
        {
            if (blendType == BlendType::Additive)
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }
            else
            {
                glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
            }
        }
    }



    Material::Material() :
        blendType(BlendType::Opaque),
        useLighting(true)
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

    void Material::Pass(int pass)
    {
        if (pass == 0)
        {
            FirstPass(blendType);
        }
        else
        {
            SecondPass(blendType);
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
