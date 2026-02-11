#pragma once

#include "Rendering/Core/InstancedMesh.h"

class GLMesh;

using ConstGLMesh = std::shared_ptr<const GLMesh>;

class GLInstancedMesh : public InstancedMesh
{
public:
    GLInstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms);
    ~GLInstancedMesh() override;
    
    void SetTransforms(const std::vector<Transform>& newTransforms) override;
    void AddTransform(const Transform& transform) override;
    void Draw() const override;
    
private:
    void InitInstanceBuffer();
    
    GLuint instanceVBO{};
    
    ConstGLMesh glMesh{};
};
