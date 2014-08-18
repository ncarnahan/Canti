#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <array>
#include <vector>
#include "OpenGL.h"
#include "Vertex.h"

namespace Graphics
{
    class Mesh
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        
        GLuint vao;
        GLuint vbo; //Buffer for storing vertex information
        GLuint eab; //Buffer for storing index information


    public:
        Mesh();

        bool LoadObjFile(const char* fileName);
        void Draw();

    private:
        //Helper functions
        bool ParseObjFile(const char* fileName,
            std::vector<Vector3>& positions,
            std::vector<Vector3>& normals,
            std::vector<Vector2>& uvs,
            std::vector<std::array<std::array<int, 3>, 3>>& faces);
        void Mesh::ConvertObjToMesh(
            std::vector<Vector3>& positions,
            std::vector<Vector3>& normals,
            std::vector<Vector2>& uvs,
            std::vector<std::array<std::array<int, 3>, 3>>& faces);
        void GenerateVao();
    };
}

#endif
