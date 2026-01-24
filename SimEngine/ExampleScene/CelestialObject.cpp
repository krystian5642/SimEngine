#include "CelestialObject.h"

#include "Components/LineComponent.h"
#include "Managers/MeshManager.h"
#include "Components/PhysicsComponent.h"
#include "Core/MathUtils.h"

CelestialObject::CelestialObject(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    SetMesh(MeshManager::Get().GetAssetByName("planet"));

    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.rotateWithCenterOfMass = false;
    physicsComponent->physicsData.linearDamping = 1.0f;
    physicsComponent->physicsData.angularDamping = 1.0f;
    physicsComponent->physicsData.angularVelocity = {0.0f, MathUtils::randomNum(-80.f, 80.0f), 0.0f};
    
    lineComponent = AddComponent<LineComponent>();
    lineComponent->followParent = true;
    lineComponent->SetThickness(7.0f);
    lineComponent->SetMaxLenght(50.0f);
}
