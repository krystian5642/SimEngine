#pragma once
#include "Scene/Objects/Entities/MeshEntity.h"

class RotatingComponent;

class Asteroid : public MeshEntity
{
public:
    Asteroid(ObjectBase* parent, Scene* scene, const std::string& name);
    
private:
    RotatingComponent* rotatingComponent;
};
