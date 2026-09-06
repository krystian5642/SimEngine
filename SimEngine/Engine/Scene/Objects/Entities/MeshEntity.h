#pragma once

#include "Entity.h"
#include "Components/MeshComponent.h"

class MeshEntity : public Entity
{
public:
    MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name);
    
    MeshComponent* meshComponent;
};
