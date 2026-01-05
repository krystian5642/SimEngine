#pragma once

#include "Component.h"

namespace SimEngine
{
    struct InputData;

    class CameraComponent : public Component
    {
    public:
        CameraComponent(ObjectBase* parent, Scene* scene, const std::string& name);
        ~CameraComponent() override;
    
        void Refresh();
        void Move(const glm::vec3& moveDelta);
        void Rotate(float pitchDelta, float yawDelta);
    
        void SetAsActiveCamera();
    
        void MoveForward(const InputData& inputData);
        void MoveBackward(const InputData& inputData);
        void MoveRight(const InputData& inputData);
        void MoveLeft(const InputData& inputData);
        void OnMouseMove(const InputData& inputData);
    
        const glm::vec3& GetForwardVector() const { return forward; }
        const glm::vec3& GetRightVector() const { return right; }
        const glm::vec3& GetPosition() const { return position; }
        const glm::mat4& GetView() const { return view; }
    
        void SetPosition(const glm::vec3& newPosition);
    
        void GetRotation(float& outPitch, float& outYaw) const;

    private:
        glm::mat4 view;
        
        glm::vec3 forward{0.0f, 0.0f, -1.0f};
        glm::vec3 up;
        glm::vec3 right;
        
        glm::vec3 position{5.0f, 5.0f, 15.0f};
        float pitch{};
        float yaw{180.0f};
    
        float cameraSpeed{10.0f};
    };
}
