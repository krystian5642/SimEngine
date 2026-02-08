#pragma once

struct MeshData
{
    std::vector<float> vertices{}; 
    std::vector<unsigned int> indices{};
    
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
    Mesh(const MeshData& meshData);
    virtual ~Mesh() {}

    virtual void Draw() const = 0;
    
protected:
    virtual void LoadGPUData(const MeshData& meshData) = 0;
    virtual void FreeGPUData() = 0;
};
