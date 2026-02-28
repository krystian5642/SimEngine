#pragma once

#include "Entity.h"
#include "Components/MeshComponent.h"

class Material;
class Mesh;

class MeshEntity : public Entity
{
public:
    MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void SetMesh(const MeshPtr& newMesh);
    void SetMaterial(const MaterialPtr& material);
    
    MeshComponent* GetMeshComponent() { return meshComponent; }

protected:
    MeshComponent* meshComponent;
};
