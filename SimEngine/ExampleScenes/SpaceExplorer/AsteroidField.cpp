#include "AsteroidField.h"

#include "Components/MeshComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

AsteroidField::AsteroidField(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    //asteroidFieldMeshComponent = AddComponent<InstancedMeshComponent>();
    
    //asteroidFieldMeshComponent->instancedMesh = InstancedMesh::CreateInstancedMesh(MeshManager::Get().GetAssetByName("planet"));
    //asteroidFieldMeshComponent->material = MaterialManager::Get().GetAssetByName("gold");
    
    GenerateAsteroids();
}

void AsteroidField::GenerateAsteroids()
{
    constexpr size_t asteroidCount = 4000;
    std::vector<Transform> asteroidTransforms;
    asteroidTransforms.reserve(asteroidCount);
    
    constexpr auto radius = 400.0f;
    constexpr auto offset = 100.0f;
    
    for (size_t i = 0; i < asteroidCount; i++)
    {
        Transform t;
        
        const auto angle = static_cast<float>(i) / static_cast<float>(asteroidCount) * 360.0f;
        const auto x = sin(glm::radians(angle)) * radius + MathUtils::RandomNum(-offset, offset);
        const auto y = MathUtils::RandomNum(-20.0f, 20.0f);
        const auto z = cos(glm::radians(angle)) * radius + MathUtils::RandomNum(-offset, offset);
        
        t.position = {x, y, z};
        t.scale = glm::vec3{MathUtils::RandomNum(0.5f, 1.5f)};
        t.rotation = glm::vec3{MathUtils::RandomNum(-360.0f, 360.0f)};
        
        auto mesh = AddComponent<MeshComponent>();
        mesh->mesh = MeshManager::Get().GetAssetByName("planet");
        mesh->material = MaterialManager::Get().GetAssetByName("gold");
        
        mesh->SetPosition(t.position);
        mesh->SetScale(t.scale);
        mesh->SetRotation(t.rotation);
        
        
        //asteroidTransforms.push_back(t);
    }
    asteroidTransforms.push_back({});
    //asteroidFieldMeshComponent->SetTransforms(asteroidTransforms);
}
