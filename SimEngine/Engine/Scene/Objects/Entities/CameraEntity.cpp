#include "CameraEntity.h"

#include "Components/CameraComponent.h"
#include "Components/InputComponent.h"

#include <GLFW/glfw3.h>

CameraEntity::CameraEntity(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    cameraComponent = AddComponent<CameraComponent>();
    
    inputComponent = AddComponent<InputComponent>();
    inputComponent->AssignAction(GLFW_KEY_W, cameraComponent, &CameraComponent::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_S, cameraComponent, &CameraComponent::MoveBackward);
    inputComponent->AssignAction(GLFW_KEY_A, cameraComponent, &CameraComponent::MoveLeft);
    inputComponent->AssignAction(GLFW_KEY_D, cameraComponent, &CameraComponent::MoveRight);
    inputComponent->AssignAction(GLFW_KEY_Q, cameraComponent, &CameraComponent::MoveUp);
    inputComponent->AssignAction(GLFW_KEY_E, cameraComponent, &CameraComponent::MoveDown);

    inputComponent->AssingOnMouseMove(cameraComponent, &CameraComponent::OnMouseMove);
    inputComponent->AssignOnMouseScroll(cameraComponent, &CameraComponent::MoveForward);
    
    tickWhenPaused = true;
}

void CameraEntity::SetAsActiveCamera()
{
    cameraComponent->SetAsActiveCamera();
}