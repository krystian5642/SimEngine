#pragma once
#include "Components/Component.h"

class ShapeComponent;
class Entity;

class PhysicsComponent : public Component
{
public:
    using Component::Component;
    
    void Init() override;
    
protected:
    Entity* parentEntity;
};

