#ifndef GRAPHICS_STATIC_MESH_H
#define GRAPHICS_STATIC_MESH_H

#include <array>
#include <vector>
#include "OpenGL.h"
#include "Vertex.h"

namespace Graphics
{
    class DrawCall;

    class StaticMesh
    {
    private:
        GLuint vao;
        GLuint vbo; //Buffer for storing vertex information
        GLuint eab; //Buffer for storing index information
        GLsizei size;


    public:
        StaticMesh();

        bool LoadObjFile(const char* fileName);
        void BufferData(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

        void FillDrawCall(DrawCall& drawCall);

    private:
        void GenerateVao();
    };
}

#endif
