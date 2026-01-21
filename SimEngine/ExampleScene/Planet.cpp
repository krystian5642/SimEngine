#include "Planet.h"

#include "Components/LineComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

Planet::Planet(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    SetMesh(MeshManager::Get().GetAssetByName("planet"));
    SetMaterial(MaterialManager::Get().GetAssetByName("moon"));

    lineComponent = AddComponent<LineComponent>();
    lineComponent->SetThickness(3.0f);
    lineComponent->SetColor({0.0f, 1.0f, 0.0f});
    lineComponent->SetMaxLenght(10.0f);


    /*gravityComponent->showTrajectory = false;
    gravityComponent->gravityData.mass = 0.001f;
    gravityComponent->gravityData.velocity = {0.0f, 2.0, 2.0f};*/
    Move({20.0f, 10.0f, -10.0f});
}

void Planet::Tick(float deltaTime)
{
    MeshEntity::Tick(deltaTime);

    lineComponent->AddPoint(GetPosition());
}
