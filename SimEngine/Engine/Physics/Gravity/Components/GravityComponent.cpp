
#include "GravityComponent.h"
#include "Physics/Gravity/Systems/GravitySystem.h"

GravityComponent::GravityComponent(const SceneObjectParams& params)
    : CompoundRigidBodyComponent(params)
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
