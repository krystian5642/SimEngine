#pragma once

#include "Components/MeshComponent.h"
#include "Core/Properties/ClassProperty.h"

DECLARE_SIMPLE_EVENT(PropertyChangedEvent)

class ShapeComponent : public MeshComponent
{
public:
    ShapeComponent(const SceneObjectParams& params
        , const std::string& meshName
        , const std::string& materialName);
    
    PROPERTY(FloatProperty, mass, 1.0f, 0.01f, 20.0f, "%.3f kg")
    
    PropertyChangedEvent propertyChangedEvent;
    
    virtual glm::mat3 CalculateLocalInertiaTensor() const = 0;
    
private:
    void OnSetMass(float newMass);
};

class BoxComponent : public ShapeComponent
{
public:
    BoxComponent(const SceneObjectParams& params);
    
    PROPERTY(Vec3Property, size, glm::vec3{1.0f})
    
    glm::mat3 CalculateLocalInertiaTensor() const override;
    
private:
    void OnSetSize(const glm::vec3& newSize);
};

class SphereComponent : public ShapeComponent
{
public:
    SphereComponent(const SceneObjectParams& params);
    
    PROPERTY(FloatProperty, radius, 0.5f, 0.01f, 5.0f, "%.3f m")
    
    glm::mat3 CalculateLocalInertiaTensor() const override;
    
private:
    void OnSetRadius(float newRadius);
};

class CylinderComponent : public ShapeComponent
{
public:
    CylinderComponent(const SceneObjectParams& params);
    
    PROPERTY(FloatProperty, radius, 0.5f, 0.01f, 5.0f, "%.3f m")
    PROPERTY(FloatProperty, height, 1.0f, 0.01f, 5.0f, "%.3f m")
    
    glm::mat3 CalculateLocalInertiaTensor() const override;
  
private:
    void OnSetRadius(float newRadius);
    void OnSetHeight(float newHeight);
};

class CapsuleComponent : public ShapeComponent
{
public:
    CapsuleComponent(const SceneObjectParams& params);
    
    PROPERTY(FloatProperty, sphereRadius, 0.5f, 0.01f, 5.0f, "%.3f m")
    PROPERTY(FloatProperty, cylinderHeight, 1.0f, 0.01f, 5.0f, "%.3f m")
    
    glm::mat3 CalculateLocalInertiaTensor() const override;
    
private:
    void OnSetSphereRadius(float newSphereRadius);
    void OnSetCylinderHeight(float newCylinderHeight);
};