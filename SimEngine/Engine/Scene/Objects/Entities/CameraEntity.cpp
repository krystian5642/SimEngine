#include "CameraEntity.h"

#include "Components/CameraComponent.h"
#include "Components/InputComponent.h"

namespace SimEngine
{
    CameraEntity::CameraEntity(ObjectBase* parent, Scene* scene, const std::string& name)
        : Entity(parent, scene, name)
    {
        cameraComponent = AddComponent<CameraComponent>();
        
        inputComponent = AddComponent<InputComponent>();
        inputComponent->AssignAction(GLFW_KEY_W, cameraComponent, &CameraComponent::MoveForward);
        inputComponent->AssignAction(GLFW_KEY_S, cameraComponent, &CameraComponent::MoveBackward);
        inputComponent->AssignAction(GLFW_KEY_A, cameraComponent, &CameraComponent::MoveLeft);
        inputComponent->AssignAction(GLFW_KEY_D, cameraComponent, &CameraComponent::MoveRight);
    
        inputComponent->AssingOnMouseMove(cameraComponent, &CameraComponent::OnMouseMove);
    }

    CameraEntity::~CameraEntity()
    {
    }
}
