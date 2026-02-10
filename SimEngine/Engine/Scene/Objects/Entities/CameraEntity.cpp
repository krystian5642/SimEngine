#include "CameraEntity.h"

#include "Components/CameraComponent.h"
#include "Components/InputComponent.h"

#include <GLFW/glfw3.h>

#include "Core/App.h"

CameraEntity::CameraEntity(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    cameraComponent = AddComponent<CameraComponent>();
    
    inputComponent = AddComponent<InputComponent>();
    inputComponent->AssignAction(GLFW_KEY_W, this, &CameraEntity::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_S, this, &CameraEntity::MoveBackward);
    inputComponent->AssignAction(GLFW_KEY_A, this, &CameraEntity::MoveLeft);
    inputComponent->AssignAction(GLFW_KEY_D, this, &CameraEntity::MoveRight);
    inputComponent->AssignAction(GLFW_KEY_E, this, &CameraEntity::MoveUp);
    inputComponent->AssignAction(GLFW_KEY_Q, this, &CameraEntity::MoveDown);

    inputComponent->AssingOnMouseMove(this, &CameraEntity::OnMouseMove);
    inputComponent->AssignOnMouseScroll(this, &CameraEntity::OnMouseScroll);
    
    tickWhenPaused = true;
}

void CameraEntity::MoveForward(const InputData& inputData)
{
    cameraComponent->Move(cameraComponent->GetForwardVector() * inputData.deltaTime * cameraSpeed);
}

void CameraEntity::MoveBackward(const InputData& inputData)
{
    cameraComponent->Move(-cameraComponent->GetForwardVector() * inputData.deltaTime * cameraSpeed);
}

void CameraEntity::MoveLeft(const InputData& inputData)
{
    cameraComponent->Move(-cameraComponent->GetRightVector() * inputData.deltaTime * cameraSpeed);
}

void CameraEntity::MoveRight(const InputData& inputData)
{
    cameraComponent->Move(cameraComponent->GetRightVector() * inputData.deltaTime * cameraSpeed);
}

void CameraEntity::MoveUp(const InputData& inputData)
{
    cameraComponent->Move({0.0f, inputData.deltaTime * cameraSpeed, 0.0f});
}

void CameraEntity::MoveDown(const InputData& inputData)
{
    cameraComponent->Move({0.0f, -inputData.deltaTime * cameraSpeed, 0.0f});
}

void CameraEntity::OnMouseMove(const InputData& inputData)
{
    if (inputData.mouseXDelta != 0.0f || inputData.mouseYDelta != 0.0f)
    {
        const auto pitchDelta = static_cast<float>(inputData.mouseYDelta) / 10.0f;
        const auto yawDelta = static_cast<float>(inputData.mouseXDelta) / 10.0f;
        
        if (!App::GetCurrentWindow()->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            cameraComponent->RotateAroundTarget(pitchDelta, yawDelta);
            return;
        }
        
        cameraComponent->Rotate(pitchDelta, yawDelta);
    }
}

void CameraEntity::OnMouseScroll(const InputData& inputData)
{
    if (inputData.mouseYDelta < 0.0f)
    {
        MoveBackward(inputData);
        return;
    }
    MoveForward(inputData);
}

void CameraEntity::SetAsActiveCamera()
{
    cameraComponent->SetAsActiveCamera();
}
