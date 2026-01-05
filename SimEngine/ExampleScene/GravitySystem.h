#pragma once

#include "Scene/Objects/Core/SceneObject.h"

class GravityComponent;

class GravitySystem : public SimEngine::SceneObject
{
public:
    GravitySystem(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name);
    
    static constexpr float G{100.0f};
    
    void Tick(float deltaTime) override;
    
    void RegisterGravityComponent(GravityComponent* gravityComponent);
    void UnregisterGravityComponent(GravityComponent* gravityComponent);
    
private:
    std::vector<GravityComponent*> gravityComponents;
};
