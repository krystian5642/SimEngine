#pragma once
#include "RigidBodyComponent.h"

class SphereComponent : public RigidBodyComponent
{
public:
    SphereComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void SetRadius(float newRadius);
    float GetRadius() const { return radius; }
    
private:
    glm::mat3 CalculateInertiaTensor() const override;
    
    float radius{1.0f};
};

class BoxComponent : public RigidBodyComponent
{
public:
    BoxComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void SetSize(const glm::vec3& newSize);
    const glm::vec3& GetSize() const { return size; }
    
private:
    glm::mat3 CalculateInertiaTensor() const override;
    
    glm::vec3 size{1.0f};
};