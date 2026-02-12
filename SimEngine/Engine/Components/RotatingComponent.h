#pragma once
#include "Component.h"

class Entity;

struct RotatingData
{
    glm::vec3 localAngularVelocity{};
    glm::vec3 orbitAngularVelocity{};
    glm::vec3 orbitCenter{};
};

class RotatingComponent : public Component
{
public:
    RotatingComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    void SetPosition(const glm::vec3& newPosition);
    const glm::vec3& GetPosition() const;
    
    void Rotate(const glm::quat& rotationDelta);
    
    RotatingData rotatingData;
    
private:
    void ApplyLocalAngularVelocity(float deltaTime);
    void ApplyOrbitAngularVelocity(float deltaTime);
    
    Entity* parentEntity{};
};
