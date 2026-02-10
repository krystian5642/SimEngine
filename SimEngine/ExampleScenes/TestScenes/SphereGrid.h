#pragma once

#include "Components/InputComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class PhysicsComponent;
class InputComponent;

class SphereGrid : public Entity
{
public:
    SphereGrid(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void CustomForce();

private:
    InputComponent* inputComponent;
    PhysicsComponent* physicsComponent;
};
