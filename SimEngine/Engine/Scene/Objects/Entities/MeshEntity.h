#pragma once

#include "Entity.h"
#include "Components/MeshComponent.h"

class Material;
class Mesh;

class MeshEntity : public Entity
{
public:
    MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name);
    
    MeshComponent* meshComponent;
};
