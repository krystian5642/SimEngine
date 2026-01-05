#pragma once

#include <vector>

namespace SimEngine
{
    struct MeshData
    {
        std::vector<float> vertices{}; 
        std::vector<unsigned int> indices{};
    };
    
    class Mesh
    {
    public:
        Mesh(const MeshData& meshData);
        virtual ~Mesh() = 0;
        
        virtual void Draw() const = 0;
        
    protected:
        virtual void LoadGPUData(const MeshData& meshData) = 0;
        virtual void FreeGPUData() = 0;
    };
}
