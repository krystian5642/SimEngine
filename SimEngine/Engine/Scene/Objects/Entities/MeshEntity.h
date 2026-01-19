#pragma once

#include "Entity.h"
#include "Components/MeshComponent.h"

class Material;
class Mesh;

class MeshEntity : public Entity
{
public:
    MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void SetMesh(const std::shared_ptr<Mesh>& newMesh);
    void SetMaterial(const std::shared_ptr<Material>& material);

protected:
    MeshComponent* meshComponent;
};
