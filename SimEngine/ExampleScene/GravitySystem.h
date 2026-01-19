#pragma once

#include "Scene/Objects/Core/SceneObject.h"

class GravityComponent;

struct GravitySystemConstants
{
    static constexpr float G{100.0f};
};

class GravitySystem : public SceneObject
{
public:
    GravitySystem(ObjectBase* parent, Scene* scene, const std::string& name);

    void Tick(float deltaTime) override;

    void RegisterGravityComponent(GravityComponent* gravityComponent);
    void UnregisterGravityComponent(GravityComponent* gravityComponent);

private:
    std::vector<GravityComponent*> gravityComponents;
};
