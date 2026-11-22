#pragma once

#include "Components/LineComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class SimpleGravityComponent;
class MeshComponent;
class ShapeComponent;
class VectorVisualizerComponent;

class CompoundRigidBody : public Entity
{
public:
    CompoundRigidBody(const SceneObjectParams& params);
    
    void PhysicsTick(float physicsDeltaTime) override;
    
    void ApplyForce(const glm::vec3& force
        , bool velocityChange = true);
    
    void ApplyForceAtLocation(const glm::vec3& force
        , const glm::vec3& location
        , bool velocityChange = true);
    
    PROPERTY(BoolProperty, showVisualizationComponents, true)
    
    PROPERTY(FunctionProperty, setLinearVelocity)
    PROPERTY(Vec3Property, linearVelocityToSet, glm::vec3(1.0f, 1.0f, 1.0f))
    
    PROPERTY(FunctionProperty, setAngularVelocity)
    PROPERTY(Vec3Property, angularVelocityToSet, glm::vec3(1.0f, 1.0f, 1.0f))
    
    PROPERTY(FunctionProperty, setBothAngularAndLinearVelocity)
    
    SimpleGravityComponent* rigidBodyComponent;
    
    VectorVisualizerComponent* linearVelocityVisComp;
    VectorVisualizerComponent* angularVelocityVisComp;
    VectorVisualizerComponent* angularMomentumVisComp;
    
    LineComponent* centerOfMassVisLine;
    MeshComponent* centerOfMassVisMesh;
    
private:
    void UpdateVisualizationComponents();
    void OnShowVisualizationComponents(bool newShowVisualizers);
    void CreateVisualizationComponents();
    
    void SetPhysicsPropertiesToReadOnly();
    
    std::vector<RenderComponent*> visualizationComponents;
};