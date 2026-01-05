#include "CameraComponent.h"

#include "Scene/Scene.h"
#include "Components/InputComponent.h"

namespace SimEngine
{
    CameraComponent::CameraComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : Component(parent, scene, name)
    {
        Refresh();
    }

    CameraComponent::~CameraComponent()
    {
    }

    void CameraComponent::Refresh()
    {
        const float yawRadians = glm::radians(yaw);
        const float pitchRadians = glm::radians(pitch);
    
        forward.x = sin(yawRadians);
        forward.y = sin(pitchRadians);
        forward.z = cos(yawRadians) * cos(pitchRadians);
    
        forward = glm::normalize(forward);
    
        right = glm::cross(forward, {0.0f, 1.0f, 0.0f});
        right = glm::normalize(right);
     
        up = glm::cross(right, forward);
    
        view = glm::lookAt(position, position + forward, up);
    }

    void CameraComponent::Move(const glm::vec3& moveDelta)
    {
        position += moveDelta;
        Refresh();
    }

    void CameraComponent::Rotate(float pitchDelta, float yawDelta)
    {
        yaw -= yawDelta;
        pitch = glm::clamp(pitch - pitchDelta, -89.9f, 89.9f);
    
        Refresh();
    }

    void CameraComponent::SetAsActiveCamera()
    {
        scene->SetActiveCamera(this);
    }

    void CameraComponent::MoveForward(const InputData& inputData)
    {
        Move(forward * cameraSpeed * inputData.deltaTime);
    }

    void CameraComponent::MoveBackward(const InputData& inputData)
    {
        Move(-forward * cameraSpeed * inputData.deltaTime);
    }

    void CameraComponent::MoveRight(const InputData& inputData)
    {
        Move(right * cameraSpeed * inputData.deltaTime);
    }

    void CameraComponent::MoveLeft(const InputData& inputData)
    {
        Move(-right * cameraSpeed * inputData.deltaTime);
    }

    void CameraComponent::OnMouseMove(const InputData& inputData)
    {
        if (inputData.mouseXDelta != 0.0f || inputData.mouseYDelta != 0.0f)
        {
            Rotate(static_cast<float>(inputData.mouseYDelta) / 10.0f
                , static_cast<float>(inputData.mouseXDelta) / 10.0f);
        }
    }

    void CameraComponent::SetPosition(const glm::vec3& newPosition)
    {
        position = newPosition;
        Refresh();
    }

    void CameraComponent::GetRotation(float& outPitch, float& outYaw) const
    {
        outPitch = pitch;
        outYaw = yaw;
    }
}