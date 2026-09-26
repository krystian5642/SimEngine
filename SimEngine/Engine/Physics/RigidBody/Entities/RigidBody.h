#pragma once

#include "Components/MeshComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class LineComponent;
class VectorVisualizerComponent;
class RigidBodyShapeComponent;

struct VelocityData
{
    glm::vec3 linearVelocity{};
    glm::vec3 angularVelocity{};
};

class CompoundRigidBody : public Entity
{
public:
    CompoundRigidBody(const SceneObjectParams& params);
    
    template<class ShapeClass>
    RigidBodyShapeComponent* AddShape(const std::string& name = "Shape Component");
    
    void Init() override;
    void PhysicsTick(float physicsDeltaTime) override;
    
    void ApplyForce(const glm::vec3& force
        , bool velocityChange = true);
    
    void ApplyTorque(const glm::vec3& force
        , const glm::vec3& location
        , bool velocityChange = true);
    
    MeshComponent* centerOfMassVisMesh;
    MeshComponent* rotationAxisVisMesh;
    
    VectorVisualizerComponent* linearVelocityVisComp;
    VectorVisualizerComponent* angularVelocityVisComp;
    
    VectorVisualizerComponent* momentumVisComp;
    VectorVisualizerComponent* angularMomentumVisComp;
    
    LineComponent* centerOfMassVisLine;
    
private:
    void UpdateVisualizationComponents();
    
    VelocityData velocity;
    
    glm::vec3 accumulatedForce{};
    glm::vec3 accumulatedTorque{};
    
    glm::mat3 totalInertiaTensor{};
    glm::mat3 invTotalInertiaTensor{};
    float totalMass{};
    float invTotalMass{};
    
    glm::vec3 centerOfMass{};
    
    std::vector<RigidBodyShapeComponent*> shapes;
};

template <class ShapeClass>
RigidBodyShapeComponent* CompoundRigidBody::AddShape(const std::string& name)
{
    assert(!GetIsInitialized());
    
    auto shape = AddComponent<ShapeClass>(name);
    shapes.push_back(shape);
    
    return shape;
}
