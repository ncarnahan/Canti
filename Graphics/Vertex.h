#ifndef GRAPHICS_VERTEX_H
#define GRAPHICS_VERTEX_H

#include <Math/Vector2.h>
#include <Math/Vector3.h>

using namespace Math;

namespace Graphics
{
    class Vertex
    {
    public:
        Vector3 position;
        Vector3 normal;
        Vector3 tangent;
        Vector2 uv;
    };
}

#endif
