#include "AsteroidField.h"

#include "Asteroid.h"
#include "Components/MeshComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/MeshEntity.h"

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
    constexpr size_t asteroidCount = 1500;
    
    std::vector<Transform> asteroidTransforms;
    asteroidTransforms.reserve(asteroidCount);
    
    constexpr auto radius = 900.0f;
    constexpr auto offset = 100.0f;
    
    for (size_t i = 0; i < asteroidCount; i++)
    {
        Transform t;
        
        const auto angle = static_cast<float>(i) / static_cast<float>(asteroidCount) * 360.0f;
        const auto x = sin(glm::radians(angle)) * radius + MathUtils::RandomNum(-offset, offset);
        const auto y = MathUtils::RandomNum(-5.0f, 5.0f);
        const auto z = cos(glm::radians(angle)) * radius + MathUtils::RandomNum(-offset, offset);
        
        auto asteroid = AddChild<Asteroid>();
        asteroid->SetPosition({x, y, z});
        
        //asteroidTransforms.push_back(t);
    }
    //asteroidTransforms.push_back({});
    //asteroidFieldMeshComponent->SetTransforms(asteroidTransforms);
}
