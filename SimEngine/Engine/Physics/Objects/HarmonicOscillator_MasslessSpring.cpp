#include "HarmonicOscillator_MasslessSpring.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/SpringComponent.h"
#include "Components/MeshComponent.h"
#include "Components/VectorVisualizerComponent.h"

HarmonicOscillator_MasslessSpring::HarmonicOscillator_MasslessSpring(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    spring = AddComponent<SpringComponent>();
    spring->SetStart(glm::vec3{0.0f});
    spring->SetEnd(glm::vec3{0.0f, -physicsData.springLength, 0.0f});
    spring->SetRadius(0.8f);
    
    auto springLine = spring->GetSpringLine();
    springLine->thickness = 4.0f;
    springLine->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    
    mesh = AddComponent<MeshComponent>();
    mesh->SetPosition(glm::vec3{0.0f, -physicsData.springLength, 0.0f});
    mesh->mesh = MeshManager::Get().GetAssetByName("sphere");
    mesh->material = MaterialManager::Get().GetAssetByName("emerald");
    mesh->SetScale(glm::vec3{1.1f});
    
    auto meshCube = AddComponent<MeshComponent>();
    meshCube->mesh = MeshManager::Get().GetAssetByName("cube");
    meshCube->material = MaterialManager::Get().GetAssetByName("bronze");
    meshCube->Move(glm::vec3(0.0f, 0.0f, 0.0f));
    meshCube->SetScale(glm::vec3{2.5f, 0.2f, 2.5f});
    
    forceVisualizer = AddComponent<VectorVisualizerComponent>();
    forceVisualizer->color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    forceVisualizer->scaleFactor = 0.2f;
}

void HarmonicOscillator_MasslessSpring::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
    
    const glm::vec3 equilibriumPosition{0.0f, -physicsData.springLength, 0.0f};
    const glm::vec3 gravityForce = glm::vec3{0.0f, physicsData.gravity, 0.0f} * physicsData.mass;
    
    const glm::vec3 force = gravityForce - physicsData.k * (mesh->GetPosition() - equilibriumPosition);
    const glm::vec3 acceleration = force / physicsData.mass;
    
    velocity += acceleration * deltaTime;
    
    const glm::vec3 deltaMove = velocity * deltaTime;
    mesh->Move(deltaMove);
    
    spring->SetEnd(spring->GetEnd() + deltaMove);
    
    forceVisualizer->SetStart(mesh->GetPosition());
    forceVisualizer->SetDirection(force);
}

float HarmonicOscillator_MasslessSpring::GetOffset() const
{
    const glm::vec3 equilibriumPosition{0.0f, -physicsData.springLength, 0.0f};
    return mesh->GetPosition().y - equilibriumPosition.y;
}
