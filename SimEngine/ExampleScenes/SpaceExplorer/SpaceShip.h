#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"

class PhysicsComponent;
class InputComponent;
struct InputData;

class SpaceShip : public MeshEntity
{
public:
    SpaceShip(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void RotateYLeft(const InputData& inputData);
    void RotateYRight(const InputData& inputData);
    
    void RotateXLeft(const InputData& inputData);
    void RotateXRight(const InputData& inputData);
    
    void RotateZLeft(const InputData& inputData);
    void RotateZRight(const InputData& inputData);
    
    void MoveForward(const InputData& inputData);
    void MoveBackward(const InputData& inputData);
    void MoveUp(const InputData& inputData);
    void MoveDown(const InputData& inputData);
    void Stop(const InputData& inputData);
    
private:
    InputComponent* inputComponent;
    PhysicsComponent* physicsComponent;
    
    float engineForce{100.0f};
    float rotationSpeed{100.0f};
};
