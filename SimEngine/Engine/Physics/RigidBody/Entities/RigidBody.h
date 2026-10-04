#pragma once

#include "Components/LineComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class MeshComponent;
class ShapeComponent;
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
    
    PROPERTY(BoolProperty, showVisualizationComponents, true)
    
    VectorVisualizerComponent* linearVelocityVisComp;
    VectorVisualizerComponent* angularVelocityVisComp;
    VectorVisualizerComponent* angularMomentumVisComp;
    
    LineComponent* centerOfMassVisLine;
    MeshComponent* centerOfMassVisMesh;
    
protected:
    ShapeComponent* shape;
    
private:
    void UpdateVisualizationComponents();
    
    void UpdateProperties();
    
    void OnShowVisualizationComponents(bool newShowVisualizers);
    
    void CreateVisualizationComponents();
    
    VelocityData velocity;
    
    glm::vec3 accumulatedForce{};
    glm::vec3 accumulatedTorque{};
    
    glm::mat3 localInertiaTensor{};
    glm::mat3 inertiaTensor{};
    glm::mat3 invInertiaTensor{};
    float mass{};
    float invMass{};
    
    glm::vec3 centerOfMass{};
    
private:
    std::vector<RenderComponent*> visualizationComponents;
};