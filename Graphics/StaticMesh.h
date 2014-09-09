#ifndef GRAPHICS_STATIC_MESH_H
#define GRAPHICS_STATIC_MESH_H

#include <array>
#include <vector>
#include "OpenGL.h"
#include "Vertex.h"

namespace Graphics
{
    class StaticMesh
    {
    private:
        GLuint vao;
        GLuint vbo; //Buffer for storing vertex information
        GLuint eab; //Buffer for storing index information
        unsigned int size;


    public:
        StaticMesh();

        bool LoadObjFile(const char* fileName);
        void BufferData(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

        void Draw();

    private:
        void GenerateVao();
    };
}

#endif
