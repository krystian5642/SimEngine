#pragma once

#include "Component.h"

struct InputData;

struct PerspectiveProjectionData
{
    float fov{45.f};
    float nearPlane{0.1f};
    float farPlane{10000.0f};
};

class CameraComponent : public Component
{
public:
    CameraComponent(ObjectBase* parent, Scene* scene, const std::string& name);

    void Refresh();
    void Move(const glm::vec3& moveDelta);
    void Rotate(float pitchDelta, float yawDelta);

    void SetAsActiveCamera();

    void MoveForward(const InputData& inputData);
    void MoveBackward(const InputData& inputData);
    void MoveRight(const InputData& inputData);
    void MoveLeft(const InputData& inputData);
    void MoveUp(const InputData& inputData);
    void MoveDown(const InputData& inputData);
    void OnMouseMove(const InputData& inputData);

    const glm::vec3& GetForwardVector() const { return forward; }
    const glm::vec3& GetRightVector() const { return right; }
    const glm::vec3& GetPosition() const { return position; }
    const glm::mat4& GetViewMatrix() const { return view; }
    glm::mat4 GetProjectionMatrix() const;

    void SetPosition(const glm::vec3& newPosition);
    
    void GetRotation(float& outPitch, float& outYaw) const;
    
    PerspectiveProjectionData perspectiveProjectionData;
    
    bool lockRotation{false};
    float cameraSpeed{10.0f};
    
private:
    glm::mat4 view;
    
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
    glm::vec3 up;
    glm::vec3 right;
    
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float pitch{};
    float yaw{180.0f};
};
