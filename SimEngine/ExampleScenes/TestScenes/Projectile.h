#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"

class PhysicsComponent;

class Projectile : public MeshEntity
{
public:
    Projectile(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Start() override;
    void Tick(float deltaTime) override;
    
    bool CanMove() const override;
    
    PhysicsComponent* GetPhysicsComponent() const { return physicsComponent; }
    
private:
    PhysicsComponent* physicsComponent;
    
    float currPitch{0.0f};
    float prevPitch{0.0f};
};
