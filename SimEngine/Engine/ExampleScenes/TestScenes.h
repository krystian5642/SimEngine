#pragma once

#include "Core/Properties/ClassProperty.h"
#include "Scene/DefaultScene.h"

class RigidBody;

#define SCENE_NAME inline const std::string

namespace SceneNames
{
    SCENE_NAME RigidBodyTest = "Rigid Body Test";
}

class RigidBodyTestScene : public DefaultScene
{
public:
    RigidBodyTestScene(const std::string& name = SceneNames::RigidBodyTest);

private:
    RigidBody* rigidBody;
    
    PROPERTY(FunctionProperty, applyTorque)
};