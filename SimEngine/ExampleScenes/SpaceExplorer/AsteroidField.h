#pragma once

#include "Components/InstancedMeshComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class AsteroidField : public Entity
{
public:
    AsteroidField(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void GenerateAsteroids();
    
private:
    InstancedMeshComponent* asteroidFieldMeshComponent;
};
