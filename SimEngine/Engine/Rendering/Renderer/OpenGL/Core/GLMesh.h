#pragma once

#include "Rendering/Core/Mesh.h"

class GLMesh : public Mesh
{
public:
    GLMesh(const MeshData& meshData);
    ~GLMesh();
    
    void Draw() const override;
    
protected:
    void LoadGPUData(const MeshData& meshData) override;
    void FreeGPUData() override;
    
private:
    GLuint VAO{}, VBO{}, IBO{};
    GLsizei drawCount{};
};
