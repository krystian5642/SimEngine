#pragma once
#include "Scene.h"

class DefaultScene : public Scene
{
public:
    DefaultScene(const std::string& name = "Default Scene");
    
protected:
    CameraComponent* camera;
};
