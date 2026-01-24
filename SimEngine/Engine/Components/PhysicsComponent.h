#pragma once

#include "Components/Component.h"

class PhysicsSystem;
class Entity;

struct PhysicsData
{
    glm::vec3 linearVelocity{};
    glm::vec3 angularVelocity{};
    glm::vec3 forceAccumulator{};
    glm::vec3 torqueAccumulator{};
    glm::vec3 centerOfMass{};
    float mass{1.0f};
    float momentOfInertia{1.0f};
    float linearDamping{0.999f};
    float angularDamping{0.6f};
    bool enableSimpleGravity{false};
    bool useBounds{true};
    bool rotateWithCenterOfMass{true};
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
    
    void Move(const glm::vec3& moveDelta);
    void Rotate(const glm::vec3& rotationDelta);
    
    bool CollidesWith(const PhysicsComponent* other) const;
    float GetRadius() const;
    
    const glm::vec3& GetPosition() const;
    
    PhysicsData physicsData;
    
protected:
    void CalculatePhysicsData();
    
    Entity* parentEntity{};
    std::vector<PhysicsSystem*> scenePhysicsSystems{};
};
