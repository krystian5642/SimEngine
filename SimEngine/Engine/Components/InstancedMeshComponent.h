#pragma once

#include "RenderComponent.h"
#include "Rendering/UniformNames.h"
#include "Rendering/Core/InstancedMesh.h"

namespace UniformNames
{
    UNIFORM_NAME useInstanceRendering = "useInstanceRendering";
}

class InstancedMeshComponent : public RenderComponent
{
public:
    InstancedMeshComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;
    
    void SetTransforms(const std::vector<Transform>& newTransforms) { instancedMesh->SetTransforms(newTransforms); }
    void AddTransform(const Transform& transform) { instancedMesh->AddTransform(transform); }
    
    InstancedMeshPtr instancedMesh;
    MaterialPtr material;
};
