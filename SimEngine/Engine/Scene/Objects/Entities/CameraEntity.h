#pragma once

#include "MeshEntity.h"
#include "Components/CameraComponent.h"

class InputComponent;
class CameraComponent;
class Projection;

class CameraEntity : public Entity
{
public:
    CameraEntity(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void SetAsActiveCamera();
    
    CameraComponent* GetCameraComponent() const { return cameraComponent; }

private:
    CameraComponent* cameraComponent;
    InputComponent* inputComponent;
};

