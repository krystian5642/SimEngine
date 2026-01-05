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
    if (parent == nullptr)
    {
        std::cout << "WARNING : GravityComponent::Init() parent is not set!!" << std::endl;
        return;
    }
    
    parentEntity = dynamic_cast<SimEngine::Entity*>(parent);
    if (!parentEntity)
    {
        std::cout << "WARNING : GravityComponent::Init() parent is not an actor!!" << std::endl;
        return;
    }
    
    gravitySystem = scene->GetObjectByClass<GravitySystem>();
    gravitySystem->RegisterGravityComponent(this);
}

void GravityComponent::Tick(float deltaTime)
{
    if (!parentEntity)
    {
        std::cout << "WARNING : GravityComponent::Tick() parent is not an actor!!" << std::endl;
        return;
    }
    
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
