#pragma once

#include "RenderComponent.h"
#include "SceneComponent.h"

class Mesh;
class Material;

class MeshComponent : public SceneComponent
{
public:
    MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name);

    void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;
    
    MeshPtr mesh;
    MaterialPtr material;
};
