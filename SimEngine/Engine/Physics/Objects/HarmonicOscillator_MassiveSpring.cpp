#include "HarmonicOscillator_MassiveSpring.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/SpringComponent.h"
#include "Components/MeshComponent.h"

HarmonicOscillator_MassiveSpring::HarmonicOscillator_MassiveSpring(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    RecreateHarmonicOscillator();
}

void HarmonicOscillator_MassiveSpring::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
    
    float mass = calculatedData.dm;
    
    glm::vec3 gravityForce = mass * glm::vec3{0.0f, physicsData.gravity, 0.0f};
    
    for (int i = 0; i < springMassPoints.size() - 2; i++)
    {
        const glm::vec3 L1 = springMassPoints[i + 1].mesh->GetPosition() - springMassPoints[i].mesh->GetPosition();
        const glm::vec3 L2 = springMassPoints[i + 2].mesh->GetPosition() - springMassPoints[i + 1].mesh->GetPosition();
        
        springMassPoints[i + 1].force = gravityForce 
        + calculatedData.dk * (calculatedData.dl - glm::length(L1)) * glm::normalize(L1)
        - calculatedData.dk * (calculatedData.dl - glm::length(L2)) * glm::normalize(L2);
    }
    
    mass += physicsData.mass;
    
    gravityForce = mass * glm::vec3{0.0f, physicsData.gravity, 0.0f};
    const glm::vec3 L = springMassPoints[springMassPoints.size() - 1].mesh->GetPosition() 
    - springMassPoints[springMassPoints.size() - 2].mesh->GetPosition();
    
    springMassPoints[springMassPoints.size() - 1].force = gravityForce 
        + calculatedData.dk * (calculatedData.dl - glm::length(L)) * glm::normalize(L);
    
    for (int i = 1; i < springMassPoints.size(); i++)
    {
         mass = i == springMassPoints.size() - 1 ? physicsData.mass + calculatedData.dm : calculatedData.dm;
        
        const glm::vec3 acceleration = springMassPoints[i].force / mass;
        springMassPoints[i].velocity += acceleration * deltaTime;
        
        const glm::vec3 deltaMove = springMassPoints[i].velocity * deltaTime;
        
        springMassPoints[i].mesh->Move(deltaMove);
        
        springs[i - 1]->SetStart(springMassPoints[i - 1].mesh->GetPosition());
        springs[i - 1]->SetEnd(springMassPoints[i].mesh->GetPosition());
        
        springMassPoints[i].force = glm::vec3{0.0f};
    }
}

float HarmonicOscillator_MassiveSpring::GetOffset() const
{
    const glm::vec3 equilibriumPosition{0.0f, -physicsData.springLength, 0.0f};
    return springMassPoints[springMassPoints.size() - 1].mesh->GetPosition().y - equilibriumPosition.y;
}

void HarmonicOscillator_MassiveSpring::SetPhysicsData(const PhysicsData& newPhysicsData)
{
    physicsData = newPhysicsData;
    RecreateHarmonicOscillator();
}

void HarmonicOscillator_MassiveSpring::ClearComponents()
{
    for (const auto& springMassPoint : springMassPoints)
    {
        springMassPoint.mesh->Destroy();
    }
    springMassPoints.clear();
    springMassPoints.reserve(physicsData.N + 1);
    
    for (auto* spring : springs)
    {
        spring->Destroy();
    }
    springs.clear();
    springs.reserve(physicsData.N);
}

void HarmonicOscillator_MassiveSpring::RecreateHarmonicOscillator()
{
    ClearComponents();
    
    calculatedData.dk = static_cast<float>(physicsData.N) * physicsData.k;
    calculatedData.dm = physicsData.springMass / static_cast<float>(physicsData.N);
    calculatedData.dl = physicsData.springLength / static_cast<float>(physicsData.N);
    
    auto meshCube = AddComponent<MeshComponent>();
    meshCube->mesh = MeshManager::Get().GetAssetByName("cube");
    meshCube->material = MaterialManager::Get().GetAssetByName("bronze");
    meshCube->Move(glm::vec3(0.0f, 0.2f, 0.0f));
    meshCube->SetScale(glm::vec3{2.5f, 0.1f, 2.5f});
    
    auto meshPoint = AddComponent<MeshComponent>();
    meshPoint->mesh = MeshManager::Get().GetAssetByName("sphere");
    meshPoint->material = MaterialManager::Get().GetAssetByName("emerald");
    meshPoint->SetPosition({0.0f, 0.0f, 0.0f});
    meshPoint->SetScale(glm::vec3{0.3f});
    
    SpringMassPoint springMassPoint;
    springMassPoint.mesh = meshPoint;
    springMassPoints.push_back(springMassPoint);
    
    for (int i = 0; i < physicsData.N; i++)
    {
        auto spring = AddComponent<SpringComponent>();
        spring->SetStart(springMassPoints[0].mesh->GetPosition() - glm::vec3(0.0f, i * calculatedData.dl , 0.0f));
        spring->SetEnd(spring->GetStart() - glm::vec3(0.0f,calculatedData.dl , 0.0f));
        spring->SetRadius(0.2f);
        
        auto springLine = spring->GetSpringLine();
        springLine->thickness = 2.0f;
        springLine->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        
        springs.push_back(spring);
        
        meshPoint = AddComponent<MeshComponent>();
        meshPoint->mesh = MeshManager::Get().GetAssetByName("sphere");
        meshPoint->material = MaterialManager::Get().GetAssetByName("emerald");
        meshPoint->SetPosition(spring->GetStart() - glm::vec3(0.0f,calculatedData.dl , 0.0f));
        meshPoint->SetScale(glm::vec3{0.3f});
        
        springMassPoint.mesh = meshPoint;
        springMassPoints.push_back(springMassPoint);
    }
    
    meshPoint = springMassPoints[springMassPoints.size() - 1].mesh;
    meshPoint->material = MaterialManager::Get().GetAssetByName("ruby");
    meshPoint->SetScale(glm::vec3{0.6f});
}
