#pragma once

#include "Components/MeshComponent.h"

class ShapeComponent : public MeshComponent
{
public:
    ShapeComponent(const SceneObjectParams& params
        , const std::string& meshName
        , const std::string& materialName);
    
    void Init() override;
    
    const glm::mat3& GetInitialInertiaTensor() const { return initialInertiaTensor; }
    float GetMass() const { return mass; }
    
protected:
    void RecalculateInertiaTensor();
    
    virtual glm::mat3 CalculateInitialInertiaTensor() const = 0;
    
    glm::mat3 initialInertiaTensor{};
    float mass{1.0f};
};

class BoxComponent : public ShapeComponent
{
public:
    BoxComponent(const SceneObjectParams& params);
    
    void SetSize(const glm::vec3& newSize)
    {
        size = newSize;
        SetScale(size);
    }
    
protected:
    glm::mat3 CalculateInitialInertiaTensor() const override;
    
    glm::vec3 size{1.0f};
};

class SphereComponent : public ShapeComponent
{
public:
    SphereComponent(const SceneObjectParams& params);
    
protected:
    glm::mat3 CalculateInitialInertiaTensor() const override;
    
    float radius{0.5f};
};

class CylinderComponent : public ShapeComponent
{
public:
    CylinderComponent(const SceneObjectParams& params);
    
protected:
    glm::mat3 CalculateInitialInertiaTensor() const override;
    
    float radius{0.5f};
    float height{1.0f};
};

class CapsuleComponent : public ShapeComponent
{
public:
    CapsuleComponent(const SceneObjectParams& params);
    
protected:
    glm::mat3 CalculateInitialInertiaTensor() const override;
    
    float sphereRadius{0.5f};
    float cylinderHeight{1.0f};
};