#include "GravityComponent.h"

#include "Components/SceneComponent.h"
#include "Scene/Scene.h"
#include "Scene/Objects/Entities/Entity.h"
#include "GravitySystem.h"

GravityComponent::GravityComponent(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
    trajectory.reserve(trajectoryLength);
}

void GravityComponent::Init()
{
    parentEntity = dynamic_cast<SimEngine::Entity*>(parent);
    
    gravitySystem = scene->GetObjectByClass<GravitySystem>();
    gravitySystem->RegisterGravityComponent(this);
}

void GravityComponent::Tick(float deltaTime)
{
    const glm::vec3 currentAcceleration = gravityData.currentGravityForce / gravityData.mass;
    gravityData.currentGravityForce = glm::vec3{0.0f};
    
    gravityData.velocity += currentAcceleration * deltaTime;
    const glm::vec3 deltaMove = gravityData.velocity * deltaTime;
    parentEntity->Move(deltaMove);
}

void GravityComponent::OnDestroy()
{
    gravitySystem->UnregisterGravityComponent(this);
}

const glm::vec3& GravityComponent::GetPosition() const
{
    return parentEntity->GetComponentByClass<SimEngine::SceneComponent>()->GetPosition();
}
