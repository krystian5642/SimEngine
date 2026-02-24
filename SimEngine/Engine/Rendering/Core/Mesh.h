#pragma once
#include "Rendering/Renderer/Renderer.h"

struct VertexData
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
};

struct MeshData
{
    std::vector<VertexData> vertices; 
    std::vector<unsigned int> indices;
    
    MeshData& operator+=(const MeshData& b)
    {
        vertices.insert(vertices.end(), b.vertices.begin(), b.vertices.end());
        indices.insert(indices.end(), b.indices.begin(), b.indices.end());
        return *this;
    }
};

class Mesh
{
public:
    Mesh(const MeshData& meshData) {}
    virtual ~Mesh() {}
    
    static MeshPtr CreateMesh(const MeshData& meshData)
    {
        return Renderer::Get()->CreateMesh(meshData);
    }
    
    virtual void Draw() const = 0;
};
