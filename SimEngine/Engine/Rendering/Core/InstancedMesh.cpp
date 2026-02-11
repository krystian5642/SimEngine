#include "InstancedMesh.h"

InstancedMesh::InstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms)
{
    InstancedMesh::SetTransforms(transforms);
}

void InstancedMesh::SetTransforms(const std::vector<Transform>& newTransforms)
{
    transformMatrices.clear();
    
    transformMatrices.reserve(newTransforms.size());
    for (const auto& transform : newTransforms)
    {
        transformMatrices.push_back(transform.CalculateModelMatrix());
    }
}

void InstancedMesh::AddTransform(const Transform& transform)
{
    transformMatrices.push_back(transform.CalculateModelMatrix());
}
