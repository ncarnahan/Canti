#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include "Texture.h"
#include "Program.h"
#include <vector>
#include <Math/Vector3.h>
using namespace Math;

namespace Graphics
{
    enum class BlendType
    {
        Opaque,
        AlphaTested,
        Transparent,
        Additive,
    };

    class Material
    {
    private:
        struct PropTexture { GLint uniform; Texture* texture; };
        struct PropFloat { GLint uniform; float value; };
        struct PropVector3 { GLint uniform; Vector3 value; };

        Program* _program;
        BlendType _blendType;

        //The first item of the pair is the shader uniform location
        std::vector<PropTexture> _textures;
        std::vector<PropFloat> _floatStorage;
        std::vector<PropVector3> _vector3Storage;

    public:
        Material();

        void Start();
        void FirstPass();
        void SecondPass();

        void SetProgram(Program& program);
        void SetBlendType(BlendType blendType) { _blendType = blendType; }
        void SetTexture(GLint location, Texture& texture);
        void SetFloat(GLint location, float value);
        void SetVector3(GLint location, Vector3 value);

        inline void SetTexture(const char* name, Texture& texture)
        {
            SetTexture(_program->GetUniformLocation(name), texture);
        }

        inline void SetFloat(const char* name, float value)
        {
            SetFloat(_program->GetUniformLocation(name), value);
        }

        inline void SetVector3(const char* name, Vector3 value)
        {
            SetVector3(_program->GetUniformLocation(name), value);
        }

        inline Program* GetProgram() { return _program; }
    };
}

#endif
