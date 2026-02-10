#pragma once

#include "Components/CameraComponent.h"
#include "Components/InputComponent.h"

class InputComponent;
class CameraComponent;

class CameraEntity : public Entity
{
public:
    CameraEntity(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void MoveForward(const InputData& inputData);
    void MoveBackward(const InputData& inputData);
    void MoveLeft(const InputData& inputData);
    void MoveRight(const InputData& inputData);
    void MoveUp(const InputData& inputData);
    void MoveDown(const InputData& inputData);
    
    void OnMouseMove(const InputData& inputData);
    void OnMouseScroll(const InputData& inputData);
    
    void SetAsActiveCamera();
    
    CameraComponent* GetCameraComponent() const { return cameraComponent; }
    
    float cameraSpeed{10.0f};

private:
    CameraComponent* cameraComponent;
    InputComponent* inputComponent;
};

