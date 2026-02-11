#pragma once

#include "Rendering/Core/Mesh.h"

class GLMesh : public Mesh
{
public:
    GLMesh(const MeshData& meshData);
    ~GLMesh() override;
    
    void Draw() const override;

    GLuint GetVAO() const { return VAO; }
    GLuint GetVBO() const { return VBO; }
    GLuint GetIBO() const { return IBO; }
    
    GLsizei GetDrawCount() const { return drawCount; }
    GLsizei GetIndicesSize() const { return indicesSize; }
    GLsizei GetIndicesCount() const { return indicesCount; }
    
private:
    GLuint VAO{}, VBO{}, IBO{};
    GLsizei drawCount{}, indicesSize{}, indicesCount{};
};
