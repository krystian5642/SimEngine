#pragma once

#include <GL/glew.h>

#include "Rendering/Core/Mesh.h"

namespace SimEngine
{
    class GLMesh : public Mesh
    {
    public:
        GLMesh(const MeshData& meshData);
        ~GLMesh() override;
        
        void Draw() const override;
        
    protected:
        void LoadGPUData(const MeshData& meshData) override;
        void FreeGPUData() override;
        
    private:
        GLuint VAO{}, VBO{}, IBO{};
        GLsizei drawCount{};
    };
}