#pragma once

#include "Components/InputComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class LineComponent;
class PhysicsComponent;
class InputComponent;
class MeshEntity;

class SphereGrid : public Entity
{
public:
    SphereGrid(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    void ApplyForce(int x, int y, const glm::vec3& force);
    
    void CustomForce(const InputData& inputData);
    
private:
    std::array<std::array<MeshEntity*, 3>, 3> grid;
    
    glm::vec3 angularVelocity{0.0f};
    glm::vec3 angularAcceleration{0.0f};
    
    glm::vec3 totalRotation{0.0f};
    
    InputComponent* inputComponent;
    PhysicsComponent* physicsComponent;
    LineComponent* lineComponent;
};
