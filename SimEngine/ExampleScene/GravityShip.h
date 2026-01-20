#pragma once

#include "Components/InputComponent.h"
#include "Scene/Objects/Entities/Entity.h"

class MeshEntity;
class InputComponent;

class GravityShip : public Entity
{
public:
    GravityShip(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void MoveUp(const InputData& inputData);
    void MoveDown(const InputData& inputData);
    
private:
    MeshEntity* left;
    MeshEntity* right;
    
    InputComponent* inputComponent;
};
