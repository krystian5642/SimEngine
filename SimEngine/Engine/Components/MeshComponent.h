#pragma once

#include "SceneComponent.h"

class Mesh;
class Material;

class MeshComponent : public SceneComponent
{
public:
    MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name);

    void Draw() const override;
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    
    float lineWidth{1.0f};
    float pointSize{3.0f};
};
