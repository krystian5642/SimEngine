#pragma once

#include "Component.h"
#include "Scene/Objects/Entities/Entity.h"

class Window;

struct PerspectiveProjectionData
{
    float fov{45.f};
    float nearPlane{0.1f};
};

class CameraComponent : public Component
{
public:
    CameraComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    void Move(const glm::vec3& moveDelta);
    void Rotate(float pitchDelta, float yawDelta);
    
    void RotateAroundTarget(float pitchDelta, float yawDelta);

    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(float newPitch, float newYaw);
    
    void SetAsActiveCamera();
    void SetFollowTarget(const Entity* target);

    const glm::vec3& GetForwardVector() const { return forward; }
    const glm::vec3& GetRightVector() const { return right; }
    const glm::vec3& GetPosition() const { return position; }
    float GetPitch() const { return pitch; }
    float GetYaw() const { return yaw; }
    const glm::mat4& GetViewMatrix() const { return view; }
    const glm::mat4& GetProjectionMatrix() const { return projection; }
    
    PerspectiveProjectionData perspectiveProjectionData;
    
    bool lockRotation{false};

private:
    void UpdateProjection(int bufferWidth, int bufferHeight);
    void UpdateView();
    void OnWindowSizeChanged(Window* window, int bufferWidth, int bufferHeight);
    
    const Entity* followTarget{nullptr};
    
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
    glm::vec3 up;
    glm::vec3 right;
    
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float pitch{};
    float yaw{180.0f};
    
    glm::mat4 projection;
    glm::mat4 view;
    
    glm::vec3 lastTargetPosition{};
};
