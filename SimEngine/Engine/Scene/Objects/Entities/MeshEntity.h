#pragma once

#include "Entity.h"
#include "Components/MeshComponent.h"

class MeshEntity : public Entity
{
public:
    MeshEntity(const SceneObjectParams& params);
    
    MeshComponent* meshComponent;
};
