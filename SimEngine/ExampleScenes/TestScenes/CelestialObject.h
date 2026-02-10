#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"

class LineComponent;
class PhysicsComponent;

class CelestialObject : public MeshEntity
{
public:
    CelestialObject(ObjectBase* parent, Scene* scene, const std::string& name);
    
    LineComponent* GetLineComponent() const { return lineComponent; }
    PhysicsComponent* GetPhysicsComponent() const { return physicsComponent; }

private:
    LineComponent* lineComponent;
    PhysicsComponent* physicsComponent;

};
