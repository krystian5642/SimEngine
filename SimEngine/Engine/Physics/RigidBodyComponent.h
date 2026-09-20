#pragma once

#include "Components/MeshComponent.h"

class Entity;

struct VelocityData
{
    glm::vec3 linearVelocity{};
    glm::vec3 angularVelocity{};
};

struct GravityData
{
    float gravity{-9.81f};
    bool enabled{false};
};

class RigidBodyComponent : public MeshComponent
{
public:
    RigidBodyComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    virtual ~RigidBodyComponent() = 0 {}

    void Init() override;
    void Tick(float deltaTime) override;
    
    void SetMass(float newMass);
    float GetMass() const { return mass; }
    
    void ApplyForce(const glm::vec3& force);
    void ApplyTorque(const glm::vec3& force, const glm::vec3& location);
    
    void MoveParent(const glm::vec3& moveDelta);
    void RotateParent(float rotationDelta, const glm::vec3& axis);
    
    const VelocityData& GetVelocityData() const { return velocity; }
    
    GravityData gravityData;
    
protected:
    void RecalculateInertiaTensor();
    
    float mass{1.0f};
    
private:
    virtual glm::mat3 CalculateInertiaTensor() const = 0;
    
    VelocityData velocity{};
    
    glm::vec3 accumulatedForce{};
    glm::vec3 accumulatedTorque{};
    
    glm::vec3 centerOfMass{};
    
    glm::mat3 inertiaTensor{};
    
    Entity* parentEntity{};
};
