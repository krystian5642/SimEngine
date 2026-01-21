#include "SphereGrid.h"

#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"
#include "Core/MathUtils.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/MeshEntity.h"

SphereGrid::SphereGrid(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    inputComponent = AddComponent<InputComponent>();
    physicsComponent = AddComponent<PhysicsComponent>();
    
    constexpr float spacing = 4.0f;
    for (int i = -1; i <= 1; i++)
    {
        const float x = static_cast<float>(i) * spacing;
        for (int j = -1; j <= 1; j++)
        {
            const float y = static_cast<float>(j) * spacing;
            auto sphere = AddChild<MeshEntity>();
            sphere->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
            sphere->SetMaterial(MaterialManager::Get().GetAssetByName("ruby"));
            sphere->SetPosition({x, y, 0.0f});
        }
    }
}

void SphereGrid::CustomForce()
{
    const glm::vec3 force = glm::vec3{MathUtils::randomNum(1.8f, 2.8f), MathUtils::randomNum(1.8f, 2.8f), 0.0f} * 7000.0f;
    const glm::vec3 point{MathUtils::randomNum(-4.0f, 4.0f), MathUtils::randomNum(-4.0f, 4.0f), 0.0f};
    physicsComponent->ApplyTorque(force, point);
}
