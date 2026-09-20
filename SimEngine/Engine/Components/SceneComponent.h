#pragma once

#include "RenderComponent.h"
#include "Core/MathUtils.h"

class SceneComponent : public RenderComponent
{
public:
    SceneComponent(const SceneObjectParams& params);
    
    void Draw() const override {}

    void Move(const glm::vec3& moveDelta);
    void Rotate(const glm::vec3& rotateDelta);
    void Rotate(float rotationDelta, const glm::vec3& axis);
    void Scale(const glm::vec3& scaleDelta);
    
    void SetRotationMode(RotationMode newRotationMode);
    void SetPosition(const glm::vec3& newPosition);
    void SetEulerRotation(const glm::vec3& newRotation);
    void SetQuatRotation(const glm::quat& newQuatRotation);
    void SetScale(const glm::vec3& newScale);
    
    void AttachComponent(SceneComponent* component);
    void DetachComponent(SceneComponent* component);

    const glm::vec3& GetForwardVector() const { return forward; }
    const glm::vec3& GetUpVector() const { return up; }
    const glm::vec3& GetRightVector() const { return right; }
    
    RotationMode GetRotationMode() const { return transform.GetRotationMode(); }
    const glm::vec3& GetPosition() const { return transform.GetPosition(); }
    const glm::vec3& GetEulerRotation() const { return transform.GetEulerRotation(); }
    const glm::quat& GetQuatRotation() const { return transform.GetQuatRotation(); }
    const glm::vec3& GetScale() const { return transform.GetScale(); }
    const glm::mat4& GetModelMatrix() const { return transform.GetModelMatrix(); }

private:
    void UpdateVectors();
    
    Transform transform;
    
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    
    std::vector<SceneComponent*> attachedComponents;
};
