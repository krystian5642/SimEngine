#pragma once

#include "Component.h"

class PhysicsSystem;
class Entity;

struct PhysicsData
{
    float mass{1.0f};
    glm::vec3 velocity{};
    bool enableGravity{false};
    bool applyFriction{true};
};

struct PhysicsBoundingBox
{
    glm::vec3 minBounds{-2.0f, -2.0f, -22.0f};
    glm::vec3 maxBounds{26.0f, 26.0f, 6.0f};
};

struct ScenePhysicsConstants
{
    static constexpr float gravity{9.81f};
    static constexpr float groundLevel{-2.0f};
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Init() override;
    void Tick(float deltaTime) override;
    void OnDestroy() override;
    
    bool CollidesWith(const PhysicsComponent* other) const;

    void ApplyForce(const glm::vec3& force);
    
    const glm::vec3& GetPosition() const;
    
    void Move(const glm::vec3& moveDelta);

    PhysicsData physicsData;
    PhysicsBoundingBox boundingBox;

private:
    glm::vec3 forceAccumulator{};
    
    Entity* parentEntity{};
    PhysicsSystem* physicsSystem{};
};
