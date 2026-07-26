#pragma once

#include "Components/Component.h"

class PhysicsSystem;
class Entity;

struct PhysicsData
{
    glm::vec3 accumulatedForce{};   
    glm::vec3 linearVelocity{};
    glm::vec3 linearAcceleration{};
    float mass{1.0f};
    float restitution{0.9f};
    float linearDamping{0.999f};
    
    bool canFall{true};
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : Component(parent, scene, name) {}

    void Init() override;
    void Tick(float deltaTime) override;
    void OnDestroy() override;
    
    void ApplyForce(const glm::vec3& force);

    void Move(const glm::vec3& moveDelta);
    
    void StopImmediately();
    
    bool CollidesWith(const PhysicsComponent* other) const;
    float GetRadius() const;
    
    const glm::vec3& GetPosition() const;
    
    PhysicsData physicsData;
    
protected:
    Entity* parentEntity{};
    std::vector<PhysicsSystem*> scenePhysicsSystems{};
};
