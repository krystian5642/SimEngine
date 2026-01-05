#include "PlanetSystem.h"

#include "GravityComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/MeshEntity.h"

PlanetSystem::PlanetSystem(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    planet = AddChild<SimEngine::MeshEntity>();
    planet->SetMesh(SimEngine::MeshManager::Get().GetAssetByName("planet"));
    planet->SetMaterial(SimEngine::MaterialManager::Get().GetAssetByName("jupiter"));
    auto mainGravComp = planet->AddComponent<GravityComponent>();
    mainGravComp->gravityData.mass = 0.8f;
    
    planet->SetScale({3.0f, 3.0f, 3.0f});
    planet->Move({10.0f, 10.0f, -10.0f});
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution dist(-80.0f, 0.0f);
    std::uniform_real_distribution dist2(-110.0f, -90.f);
    
    for (int i = 0; i < 1; i++)
    {
        auto child = AddChild<SimEngine::MeshEntity>();
        
        child->SetMesh(SimEngine::MeshManager::Get().GetAssetByName("planet"));
        child->SetMaterial(SimEngine::MaterialManager::Get().GetAssetByName("moon"));

        auto physicsComp = child->AddComponent<GravityComponent>();
        physicsComp->showTrajectory = false;
        physicsComp->gravityData.mass = 0.001f;
        physicsComp->gravityData.velocity = {0.0f, 2.0, 2.0f};
        
        child->Move({20.0f, 10.0f, -10.0f});
        //child->Move({0.0f, 100.0f, -100.0f});
    }
}

void PlanetSystem::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
    
    planet->Rotate({0.0f, 60.0f * deltaTime, 0.0f});
    
    childEntities.ForEach([deltaTime](Entity* child, int index)
    {
        child->Rotate({0.0f, 60.0f * deltaTime, 0.0f});
    });
}
