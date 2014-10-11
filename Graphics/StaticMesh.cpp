#include "StaticMesh.h"
#include "Program.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <Utils/Parse.h>
using namespace Utils;

namespace Graphics
{
    //Helper function forward declarations
    bool ParseObjFile(const char* fileName,
        std::vector<Vector3>& positions,
        std::vector<Vector3>& normals,
        std::vector<Vector2>& uvs,
        std::vector<std::array<std::array<int, 3>, 3>>& faces);
    void CalculateTangents(
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices);
    void ConvertObjToMesh(
        //In
        std::vector<Vector3>& positions,
        std::vector<Vector3>& normals,
        std::vector<Vector2>& uvs,
        std::vector<std::array<std::array<int, 3>, 3>>& faces,
        //Out
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices);



    StaticMesh::StaticMesh()
    {
    }
    

    bool StaticMesh::LoadObjFile(const char* fileName)
    {
        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector2> uvs;
        //Vector of 3x3 arrays
        //The outer array is the vertices of a triangle
        //The inner array stores indices of position, uv, and normal
        std::vector<std::array<std::array<int, 3>, 3>> faces;

        //Parse the obj file into an in-memory representation
        if (!ParseObjFile(fileName, positions, normals, uvs, faces))
        {
            return false;
        }


        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        //Convert the in-memory OBJ into vertices and indices
        ConvertObjToMesh(positions, normals, uvs, faces, vertices, indices);


        //Calculate tangents if possible
        if (uvs.size() > 0 && normals.size() > 0)
        {
            CalculateTangents(vertices, indices);
        }


        //Generate a VAO, create VBOs, etc.
        BufferData(vertices, indices);
        
        return true;
    }

    //Generates the VAO, generates two VBOs (one for vertices, one for
    //indices), and sets up the attribute pointers for common attributes.
    void StaticMesh::BufferData(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    {
        //Generate the VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //Generate the vertices VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        //Generate the indices EAB
        glGenBuffers(1, &eab);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
        size = indices.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * size, &indices[0], GL_STATIC_DRAW);

        //Assign the attributes
        glEnableVertexAttribArray((int)ProgramAttribute::Position);
        glVertexAttribPointer((int)ProgramAttribute::Position,
            3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray((int)ProgramAttribute::Normal);
        glVertexAttribPointer((int)ProgramAttribute::Normal,
            3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray((int)ProgramAttribute::Tangent);
        glVertexAttribPointer((int)ProgramAttribute::Tangent,
            3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray((int)ProgramAttribute::UV);
        glVertexAttribPointer((int)ProgramAttribute::UV,
            2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    }

    void StaticMesh::Draw()
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    }



    //Helper function to parse an obj file into an in-memory representation
    bool ParseObjFile(const char* fileName,
        std::vector<Vector3>& positions,
        std::vector<Vector3>& normals,
        std::vector<Vector2>& uvs,
        std::vector<std::array<std::array<int, 3>, 3>>& faces)
    {
        std::string line;
        std::ifstream file;

        //Try to open the file
        file.open(fileName);
        if (!file.is_open())
        {
            std::cout << "Failed to open file " << fileName << ".\n";
            return false;
        }

        //Read line by line
        while (std::getline(file, line))
        {
            std::vector<std::string> items = Parse::Split(line);

            if (items.size() == 0)
            {
                //Empty line; ignore
                continue;
            }
            //Vertex position line
            else if (items[0] == "v")
            {
                Vector3 position;
                for (int i = 0; i < 3; i++)
                {
                    position[i] = Parse::ToFloat(items[i + 1]);
                }
                positions.push_back(position);
            }
            //UV coordinate line
            else if (items[0] == "vt")
            {
                Vector2 uv;
                for (int i = 0; i < 2; i++)
                {
                    uv[i] = Parse::ToFloat(items[i + 1]);
                }
                uvs.push_back(uv);
            }
            //Normal line
            else if (items[0] == "vn")
            {
                Vector3 normal;
                for (int i = 0; i < 3; i++)
                {
                    normal[i] = Parse::ToFloat(items[i + 1]);
                }
                normals.push_back(normal);
            }
            //Face line
            else if (items[0] == "f")
            {
                std::array<std::array<int, 3>, 3> face;
                for (unsigned int v = 0; v < 3; v++)
                {
                    //Split the item into vertex, uv, and normal
                    auto vItems = Parse::Split(items[v + 1], '/');

                    //Subtract 1 from each of the indices because an OBJ file is 1-indexed

                    //Read vertex
                    face[v][0] = Parse::ToInt(vItems[0]) - 1;

                    //Read uv if it exists
                    if (vItems.size() > 1 && vItems[1] != "")
                        face[v][1] = Parse::ToInt(vItems[1]) - 1;
                    else
                        face[v][1] = -1;

                    //Read normal if it exists
                    if (vItems.size() > 2)
                        face[v][2] = Parse::ToInt(vItems[2]) - 1;
                    else
                        face[v][2] = -1;
                }
                faces.push_back(face);
            }
        }

        return true;
    }


    void CalculateTangents(
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices)
    {
        std::vector<Vector3> tan1(vertices.size());
        std::vector<Vector3> tan2(vertices.size());

        //http://www.terathon.com/code/tangent.html
        for (size_t index = 0; index < indices.size(); index += 3)
        {
            size_t index1 = indices[index];
            size_t index2 = indices[index + 1];
            size_t index3 = indices[index + 2];

            Vector3 pos1 = vertices[index1].position;
            Vector3 pos2 = vertices[index2].position;
            Vector3 pos3 = vertices[index3].position;

            Vector2 uv1 = vertices[index1].uv;
            Vector2 uv2 = vertices[index2].uv;
            Vector2 uv3 = vertices[index3].uv;

            Vector3 xyz1 = pos2 - pos1;
            Vector3 xyz2 = pos3 - pos1;

            Vector2 st1 = uv2 - uv1;
            Vector2 st2 = uv3 - uv1;

            float r = 1.0f / (st1.x * st2.y - st2.x * st1.y);
            Vector3 sdir(
                (st2.y * xyz1.x - st1.y * xyz2.x) * r,
                (st2.y * xyz1.y - st1.y * xyz2.y) * r,
                (st2.y * xyz1.z - st1.y * xyz2.z) * r);
            Vector3 tdir(
                (st1.x * xyz2.x - st2.x * xyz1.x) * r,
                (st1.x * xyz2.y - st2.x * xyz1.y) * r,
                (st1.x * xyz2.z - st2.x * xyz1.z) * r);

            tan1[index1] += sdir;
            tan1[index2] += sdir;
            tan1[index3] += sdir;

            tan2[index1] += sdir;
            tan2[index2] += sdir;
            tan2[index3] += sdir;
        }

        for (size_t i = 0; i < vertices.size(); i++)
        {
            Vector3 normal = vertices[i].normal;
            Vector3 tangent = tan1[i];

            tangent = (tangent - normal * Vector3::Dot(normal, tangent)).Normalized();

            if (Vector3::Dot(Vector3::Cross(normal, tangent), tan2[i]) < 0.0f)
            {
                tangent = -tangent;
            }

            vertices[i].tangent = tangent;
        }
    }


    struct VertexIndicesHash
    {
        std::size_t operator()(const std::array<int, 3>& a) const
        {
            return a[0] ^ (a[1] << 8) ^ (a[2] << 8);
        }
    };

    struct VertexIndicesEqual
    {
        bool operator()(const std::array<int, 3>& lhs, const std::array<int, 3>& rhs) const 
        {
            return lhs[0] == rhs[0] &&
                lhs[1] == rhs[1] && 
                lhs[2] == rhs[2];
        }
    };

    //Helper function to turn the obj data into a Vertex array and an index array
    void ConvertObjToMesh(
        //In
        std::vector<Vector3>& positions,
        std::vector<Vector3>& normals,
        std::vector<Vector2>& uvs,
        std::vector<std::array<std::array<int, 3>, 3>>& faces,
        //Out
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices)
    {
        //This maps vertices to their indices
        std::unordered_map<std::array<int, 3>, int, VertexIndicesHash, VertexIndicesEqual> vertexToIndex;

        for (auto& face : faces)
        {
            //Create the vertices from the face
            for (int i = 0; i < 3; i++)
            {
                auto vertexIndices = face[i];

                int index;
                auto iter = vertexToIndex.find(vertexIndices);
                if (iter == vertexToIndex.end())
                {
                    Vertex vertex;
                    vertex.position = positions[vertexIndices[0]];
                    if (vertexIndices[1] != -1)
                    {
                        vertex.uv = uvs[vertexIndices[1]];
                    }
                    if (vertexIndices[2] != -1)
                    {
                        vertex.normal = normals[vertexIndices[2]];
                    }

                    //It hasn't already been added to the list
                    vertices.push_back(vertex);
                    index = vertices.size() - 1;

                    //Add to the hashmap for later
                    vertexToIndex.insert(
                        std::pair<std::array<int, 3>, int>(vertexIndices, index));
                }
                else
                {
                    //It already exists, use the existing index
                    index = iter->second;
                }

                //Add the index to the list
                indices.push_back(index);
            }
        }
    }
}
