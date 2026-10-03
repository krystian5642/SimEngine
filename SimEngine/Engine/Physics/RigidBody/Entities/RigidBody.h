#pragma once

#include "Scene/Objects/Entities/Entity.h"

class CapsuleComponent;
class VectorVisualizerComponent;

struct VelocityData
{
    glm::vec3 linearVelocity{};
    glm::vec3 angularVelocity{};
};

class RigidBody : public Entity
{
public:
    RigidBody(const SceneObjectParams& params);
    
    void Init() override;
    void PhysicsTick(float physicsDeltaTime) override;
    
    void ApplyForce(const glm::vec3& force
        , bool velocityChange = true);
    
    void ApplyTorque(const glm::vec3& force
        , const glm::vec3& location
        , bool velocityChange = true);
    
    VectorVisualizerComponent* vis;
    
protected:
    CapsuleComponent* box;
    
private:
    void UpdateVisualizationComponents();
    
    VelocityData velocity;
    
    glm::vec3 accumulatedForce{};
    glm::vec3 accumulatedTorque{};
    
    glm::mat3 initialInertiaTensor{};
    glm::mat3 inertiaTensor{};
    glm::mat3 invInertiaTensor{};
    float mass{};
    float invMass{};
    
    glm::vec3 centerOfMass{};
};