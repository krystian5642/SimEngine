#pragma once
#include "Components/SpringComponent.h"

class MovementComponent;

class PhysicsSpringComponent : public SpringComponent
{
public:
    PhysicsSpringComponent(const SceneObjectParams& params);
    
    void PhysicsTick(float physicsDeltaTime) override;
    
    PROPERTY(FloatProperty, baseLenght, 5.0f, 0.01f, 30.0f, "%.3f m")
    PROPERTY(FloatProperty, springConstant, 10.0f, 0.01f, 30.0f, "%.3f N/m")
    
    void AttachComponentToStart(MovementComponent* movementComponent);
    void AttachComponentToEnd(MovementComponent* movementComponent);
    
private:
    MovementComponent* startComponent{nullptr};
    MovementComponent* endComponent{nullptr};
};
