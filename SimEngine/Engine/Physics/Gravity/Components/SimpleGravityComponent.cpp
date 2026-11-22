
#include "SimpleGravityComponent.h"

#include "Physics/Gravity/Systems/GravitySystem.h"
#include "Physics/Gravity/Systems/SimpleGravitySystem.h"

SimpleGravityComponent::SimpleGravityComponent(const SceneObjectParams& params)
    : CompoundRigidBodyComponent(params)
{
}

void SimpleGravityComponent::Init()
{
    CompoundRigidBodyComponent::Init();
    
    Register<SimpleGravitySystem>();
}

void SimpleGravityComponent::OnDestroy()
{
    CompoundRigidBodyComponent::OnDestroy();
    
    Unregister<SimpleGravitySystem>();
}
