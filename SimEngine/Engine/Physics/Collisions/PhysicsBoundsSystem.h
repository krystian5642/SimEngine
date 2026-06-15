#pragma once

#include "Physics/PhysicsSystem.h"

struct PhysicsBoundingBox
{
    glm::vec3 minBounds{-2.0f, -2.0f, -22.0f};
    glm::vec3 maxBounds{26.0f, 26.0f, 6.0f};
};

class PhysicsBoundsSystem : public PhysicsSystem
{
public:
    PhysicsBoundsSystem(ObjectBase* parent, Scene* scene, const std::string& name)
        : PhysicsSystem(parent, scene, name) {}
    
    void Tick(float deltaTime) override;
    
    PhysicsBoundingBox boundingBox;
};
