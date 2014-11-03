#ifndef GRAPHICS_DRAW_CALL_H
#define GRAPHICS_DRAW_CALL_H

#include "OpenGL.h"
#include <Math/Matrix4x4.h>
using namespace Math;

namespace Graphics
{
    class Material;
    class Light;

    class DrawCall
    {
    public:
        int pass;

        Matrix4x4 modelMatrix;

        GLenum drawMode;
        GLuint vao;
        GLsizei size;

        Material* material;
        Light* light;
        Vector3 ambientLight;


        DrawCall() :
            pass(0),
            drawMode(GL_TRIANGLES),
            vao(0),
            size(0),
            material(nullptr),
            light(nullptr)
        {
        }
    };
}

#endif
