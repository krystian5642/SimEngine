#pragma once

#include "Rendering/Core/Mesh.h"

class GLMesh : public Mesh
{
public:
    GLMesh(const MeshData& meshData);
    ~GLMesh() override;
    
    void Draw() const override;

private:
    GLuint VAO{}, VBO{}, IBO{};
    GLsizei drawCount{};
};
