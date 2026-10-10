
#include "GravityComponent.h"
#include "Physics/Gravity/Systems/GravitySystem.h"

GravityComponent::GravityComponent(const SceneObjectParams& params)
    : RigidBodyComponent(params)
{
    canRotate = false;
}

void GravityComponent::Init()
{
    MovementComponent::Init();
    
    Register<GravitySystem>();
}

void GravityComponent::OnDestroy()
{
    MovementComponent::OnDestroy();
    
    Unregister<GravitySystem>();
}
