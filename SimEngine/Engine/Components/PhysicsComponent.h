#pragma once

#include "Components/Component.h"

class PhysicsSystem;
class Entity;

struct PhysicsData
{
    glm::vec3 linearVelocity{};
    glm::vec3 angularVelocity{};
    glm::vec3 linearAcceleration{};
    glm::vec3 angularAcceleration{};
    glm::vec3 centerOfMass{};
    float mass{1.0f};
    glm::mat3 invertedInertiaTensor{};
    float linearDamping{0.999f};
    float angularDamping{0.6f};
    bool enableSimpleGravity{false};
    bool useBounds{true};
    bool rotateWithCenterOfMass{true};
    bool canBounce{true};
    bool stopAtCollision{false};
    bool enableCollision{true};
    glm::bvec3 physicsLinearConstraints{false, false, false};
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name);

    void Init() override;
    void Start() override;
    void Tick(float deltaTime) override;
    void OnDestroy() override;
    
    void ApplyForce(const glm::vec3& force);
    void ApplyTorque(const glm::vec3& force, const glm::vec3& point);
    
    void ApplyImpulse(const glm::vec3& impulse);
    void ApplyAngularImpulse(const glm::vec3& impulse);
    
    void Move(const glm::vec3& moveDelta);
    
    void StopImediately();
    
    bool CollidesWith(const PhysicsComponent* other) const;
    float GetRadius() const;
    
    const glm::vec3& GetPosition() const;
    
    PhysicsData physicsData;
    float radiusMultiplier{1.0f};
    
protected:
    void CalculatePhysicsData();
    
    void ApplyAngularVelocity(float deltaTime);
    
    Entity* parentEntity{};
    std::vector<PhysicsSystem*> scenePhysicsSystems{};
};
