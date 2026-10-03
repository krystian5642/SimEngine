#pragma once
#include "Components/Component.h"

class Entity;

class PhysicsComponent : public Component
{
public:
    using Component::Component;
    
    void Init() override;
    void OnDestroy() override;
    
protected:
    Entity* parentEntity{};
    //std::vector<PhysicsSystem*> scenePhysicsSystems{};
};
