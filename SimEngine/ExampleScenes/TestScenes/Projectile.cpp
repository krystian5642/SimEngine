#include "Projectile.h"

#include "Components/PhysicsComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

Projectile::Projectile(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    SetMaterial(MaterialManager::Get().GetAssetByName("turquoise"));

    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.enableSimpleGravity = true;

    SetScale({0.4f, 0.4f, 2.0f});
}

void Projectile::Start()
{
    MeshEntity::Start();
    
    const auto& linearVelocity = physicsComponent->physicsData.linearVelocity;
    
    const auto horizontalVelCurr = glm::vec2{linearVelocity.x, linearVelocity.z};
    const auto horizontalSpeedCurr = glm::length(horizontalVelCurr);
    prevPitch = horizontalSpeedCurr > 0.0f ? glm::degrees(glm::atan(linearVelocity.y / horizontalSpeedCurr)) : 0.0f;
}

void Projectile::Tick(float deltaTime)
{
    MeshEntity::Tick(deltaTime);
    
    if (!CanMove())
    {
        return;
    }
    
    const auto& linearVelocity = physicsComponent->physicsData.linearVelocity;
    
    const auto horizontalVelCurr = glm::vec2{linearVelocity.x, linearVelocity.z};
    const auto horizontalSpeedCurr = glm::length(horizontalVelCurr);
    currPitch = horizontalSpeedCurr > 0.0f ? glm::degrees(glm::atan(linearVelocity.y / horizontalSpeedCurr)) : 0.0f;

    Rotate({currPitch - prevPitch, 0.0f, 0.0f});
    
    
    prevPitch = currPitch;
}

bool Projectile::CanMove() const
{
    return GetPosition().y > -2.5f;
}
