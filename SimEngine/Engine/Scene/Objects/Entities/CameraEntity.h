#pragma once

#include "MeshEntity.h"

namespace SimEngine
{
    class InputComponent;
    class CameraComponent;

    class CameraEntity : public Entity
    {
    public:
        CameraEntity(ObjectBase* parent, Scene* scene, const std::string& name);
        ~CameraEntity() override;
    
        const CameraComponent* GetCameraComponent() const { return cameraComponent; }
    
    private:
        CameraComponent* cameraComponent;
        InputComponent* inputComponent;
    };

}