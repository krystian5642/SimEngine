#pragma once

#include "Rendering/Renderer/Renderer.h"
#include "Core/MathUtils.h"

struct MeshData;

class InstancedMesh
{
public:
    InstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms = {});
    virtual ~InstancedMesh() {}
    
    static InstancedMeshPtr CreateInstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms = {})
    {
        return Renderer::Get()->CreateInstancedMesh(mesh, transforms);
    }
    
    virtual void SetTransforms(const std::vector<Transform>& newTransforms);
    virtual void AddTransform(const Transform& transform);
    virtual void Draw() const = 0;
    
protected:
    std::vector<glm::mat4> transformMatrices;
};
