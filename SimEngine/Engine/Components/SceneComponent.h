#pragma once

#include "RenderComponent.h"

struct Transform
{
    glm::vec3 position{};
    glm::vec3 rotation{};
    glm::vec3 scale{1.0f};
};

class SceneComponent : public RenderComponent
{
public:
    SceneComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;

    void Move(const glm::vec3& moveDelta);
    void Rotate(const glm::vec3& rotationDelta);
    void Scale(const glm::vec3& scaleDelta);
    
    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(const glm::vec3& newRotation);
    void SetScale(const glm::vec3& newScale);

    void AttachComponent(SceneComponent* component);
    void DetachComponent(SceneComponent* component);

    const glm::vec3& GetForwardVector() const { return forward; }
    const glm::vec3& GetUpVector() const { return up; }
    const glm::vec3& GetRightVector() const { return right; }
    
    const glm::vec3& GetPosition() const { return transform.position; }
    const glm::vec3& GetRotation() const { return transform.rotation; }
    const glm::vec3& GetScale() const { return transform.scale; }
    const glm::mat4& GetModelMatrix() const { return modelMatrix; }

private:
    void UpdateVectors();
    void UpdateModelMatrix();
    
    Transform transform;
    glm::mat4 modelMatrix;
    
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    
private:
    std::vector<SceneComponent*> attachedComponents;
};
