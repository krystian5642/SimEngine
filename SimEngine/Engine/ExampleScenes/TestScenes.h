#pragma once

#include "Scene/DefaultScene.h"

class CompoundRigidBody;

#define SCENE_NAME inline const std::string

namespace SceneNames
{
    SCENE_NAME RigidBodyTest = "Rigid Body Test";
}

class RigidBodyTestScene : public DefaultScene
{
public:
    RigidBodyTestScene(const std::string& name = SceneNames::RigidBodyTest);
    
    void Tick(float deltaTime) override;
    
    void DrawUI() override;
    
private:
    CompoundRigidBody* rigidBody;
};