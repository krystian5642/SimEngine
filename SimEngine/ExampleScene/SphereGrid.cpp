#include "SphereGrid.h"

#include <GLFW/glfw3.h>

#include "Components/InputComponent.h"
#include "Components/LineComponent.h"
#include "Components/PhysicsComponent.h"
#include "Core/MathUtils.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/MeshEntity.h"

SphereGrid::SphereGrid(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
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
            
            grid[i + 1][j + 1] = sphere;
        }
    }
    
    lineComponent = AddComponent<LineComponent>();
    
    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.enableGravity = false;
    physicsComponent->physicsData.applyFriction = false;
    physicsComponent->physicsData.useBounds = false;
    physicsComponent->physicsData.mass = 1000.0f;
    
    inputComponent = AddComponent<InputComponent>();
    inputComponent->AssignAction(GLFW_KEY_P, this, &SphereGrid::CustomForce);
}

void SphereGrid::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
    
    angularVelocity *= 0.999f;
    
    angularVelocity += angularAcceleration * deltaTime;
    angularAcceleration = glm::vec3{0.0f};
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            const auto sphere = grid[i][j];
            
            const auto rotDelta= angularVelocity * deltaTime;
    
            glm::mat4 model{1.0f};
    
            model = glm::rotate(model, glm::radians(rotDelta.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
            const auto prevPosition = sphere->GetPosition() - grid[1][1]->GetPosition();
            const auto newPosition = glm::vec3{model * glm::vec4{prevPosition, 1.0f}};
            
            sphere->SetPosition(newPosition + grid[1][1]->GetPosition());
        }
    }
    
    if (glm::abs(angularVelocity.z) > 0.1f)
    {
        lineComponent->AddPoint(grid[1][1]->GetPosition());
    }
}

void SphereGrid::ApplyForce(int x, int y, const glm::vec3& force)
{
    const auto* sphere = grid[x][y];
    
    const auto torque = glm::cross(sphere->GetPosition(), force);
    const float mass = 0.05f;
    
    float momentOfInertia = 0.0f;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            momentOfInertia += mass * glm::dot(grid[i][j]->GetPosition(), grid[i][j]->GetPosition());
        }
    }
    
    angularAcceleration = torque / momentOfInertia;
    
    physicsComponent->ApplyForce(force);
}

void SphereGrid::CustomForce(const InputData& inputData)
{
    ApplyForce(0, 2, glm::vec3{MathUtils::randomNum(1.8f, 2.8f), MathUtils::randomNum(1.8f, 2.8f), 0.0f} * 7000000.0f);
}
