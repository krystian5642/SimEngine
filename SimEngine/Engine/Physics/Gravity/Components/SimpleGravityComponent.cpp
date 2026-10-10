
#include "SimpleGravityComponent.h"

#include "Physics/Gravity/Systems/GravitySystem.h"
#include "Physics/Gravity/Systems/SimpleGravitySystem.h"

SimpleGravityComponent::SimpleGravityComponent(const SceneObjectParams& params)
    : RigidBodyComponent(params)
{
}

void SimpleGravityComponent::Init()
{
    RigidBodyComponent::Init();
    
    Register<SimpleGravitySystem>();
}

void SimpleGravityComponent::OnDestroy()
{
    RigidBodyComponent::OnDestroy();
    
    Unregister<SimpleGravitySystem>();
}
