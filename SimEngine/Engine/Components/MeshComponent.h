#pragma once

#include "SceneComponent.h"

class Mesh;
class Material;

class MeshComponent : public SceneComponent
{
public:
    using SceneComponent::SceneComponent;

    void Draw() const override;
    bool IsTransparent() const override;
    
    glm::vec3 GetUIColor() const override;
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};
