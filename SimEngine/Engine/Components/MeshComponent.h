#pragma once

#include "SceneComponent.h"

class Mesh;
class Material;

class MeshComponent : public SceneComponent
{
public:
    using SceneComponent::SceneComponent;

    void Draw() const override;
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};
