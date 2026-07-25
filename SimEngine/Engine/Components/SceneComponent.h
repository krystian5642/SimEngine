#pragma once

#include "RenderComponent.h"
#include "CoordinateSystems/CoordinateSystems.h"
#include "Core/MathUtils.h"

class SceneComponent : public RenderComponent
{
public:
    SceneComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Draw() const override {}

    void Move(const glm::vec3& moveDelta);
    void Rotate(const glm::vec3& rotationDelta);
    void Rotate(const glm::quat& rotationDelta);
    void Scale(const glm::vec3& scaleDelta);
    
    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(const glm::vec3& newRotation);
    void SetScale(const glm::vec3& newScale);
    
    void AttachComponent(SceneComponent* component);
    void DetachComponent(SceneComponent* component);

    const glm::vec3& GetForwardVector() const { return forward; }
    const glm::vec3& GetUpVector() const { return up; }
    const glm::vec3& GetRightVector() const { return right; }
    
    const glm::vec3& GetPosition(bool getCartesianPosition = false) const { return coordinateSystem->GetPosition(getCartesianPosition); }
    const glm::vec3& GetRotation() const { return transform.rotation; }
    const glm::vec3& GetScale() const { return transform.scale; }
    const glm::mat4& GetModelMatrix() const { return modelMatrix; }
    
    void SetCoordinateSystemType(CoordinateSystemType newType);
    
private:
    void UpdateVectors();
    void UpdateModelMatrix();
    
    Transform transform;
    glm::mat4 modelMatrix;
    
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    
    std::unique_ptr<CoordinateSystem> coordinateSystem;
    
    std::vector<SceneComponent*> attachedComponents;
};
