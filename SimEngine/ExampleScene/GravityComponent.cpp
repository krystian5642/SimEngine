#include "GravityComponent.h"

#include "Components/SceneComponent.h"
#include "Scene/Scene.h"
#include "Scene/Objects/Entities/Entity.h"
#include "GravitySystem.h"

GravityComponent::GravityComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
    trajectory.reserve(trajectoryLength);
}

void GravityComponent::Init()
{
    parentEntity = dynamic_cast<Entity*>(parent);

    gravitySystem = scene->GetObjectByClass<GravitySystem>();
    gravitySystem->RegisterGravityComponent(this);
}

void GravityComponent::Tick(float deltaTime)
{
    deltaTime = std::min(deltaTime, 1.0f / 120.0f);
    
    const auto mask = glm::vec3{1.0f} - glm::vec3(gravityConstraints);
    
    const auto currentAcceleration = gravityData.currentGravityForce * mask / gravityData.mass;
    gravityData.currentGravityForce = glm::vec3{0.0f};
    
    gravityData.velocity += currentAcceleration * deltaTime;
    const auto deltaMove = gravityData.velocity * deltaTime;
    parentEntity->Move(deltaMove);
}

void GravityComponent::OnDestroy()
{
    gravitySystem->UnregisterGravityComponent(this);
}

const glm::vec3& GravityComponent::GetPosition() const
{
    return parentEntity->GetComponentByClass<SceneComponent>()->GetPosition();
}
