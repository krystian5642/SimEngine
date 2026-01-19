#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"

class LineComponent;
class PhysicsComponent;
struct InputData;
class InputComponent;

class BallLauncher : public MeshEntity
{
public:
    BallLauncher(ObjectBase* parent, Scene* scene, const std::string& name);

    void Tick(float deltaTime) override;

protected:
    void RotateBarrelLeft(const InputData& inputData);
    void RotateBarrelRight(const InputData& inputData);
    void Fire(const InputData& inputData);
    void DeleteLastBall(const InputData& inputData);

    void RotateLeft(const InputData& inputData);
    void RotateRight(const InputData& inputData);
    void MoveForward(const InputData& inputData);
    void MoveBackward(const InputData& inputData);

    MeshComponent* barrelComponent;
    InputComponent* inputComponent;
    PhysicsComponent* physicsComponent;
    LineComponent* lineComponent;

    float rotationSpeed{60.0f};
    float moveSpeed{10.0f};
    double fireCooldown{0.2};
private:
    glm::vec3 GetBallSpawnPosition() const;
    glm::vec3 GetBallStartVelocity() const;

    void UpdateTrajectory();

    double lastFireTime{};

    SceneObjectHandle lastBallHandle{};
};
