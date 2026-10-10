#pragma once

#include "Components/LineComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class SimpleGravityComponent;
class MeshComponent;
class ShapeComponent;
class VectorVisualizerComponent;

class RigidBody : public Entity
{
public:
    RigidBody(const SceneObjectParams& params);
    
    void Init() override;
    void PhysicsTick(float physicsDeltaTime) override;
    
    void ApplyForce(const glm::vec3& force
        , bool velocityChange = true);
    
    void ApplyForceAtLocation(const glm::vec3& force
        , const glm::vec3& location
        , bool velocityChange = true);
    
    PROPERTY(BoolProperty, showVisualizationComponents, true)
    
    SimpleGravityComponent* rigidBodyComponent;
    ShapeComponent* shape;
    
    VectorVisualizerComponent* linearVelocityVisComp;
    VectorVisualizerComponent* angularVelocityVisComp;
    VectorVisualizerComponent* angularMomentumVisComp;
    
    LineComponent* centerOfMassVisLine;
    MeshComponent* centerOfMassVisMesh;
    
private:
    void UpdateVisualizationComponents();
    void UpdateProperties();
    void OnShowVisualizationComponents(bool newShowVisualizers);
    void CreateVisualizationComponents();
    
    std::vector<RenderComponent*> visualizationComponents;
};