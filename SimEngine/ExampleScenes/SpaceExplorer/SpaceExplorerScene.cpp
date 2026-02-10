#include "SpaceExplorerScene.h"

#include "SpaceShip.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/CameraEntity.h"
#include "Scene/Systems/Physics/GravitySystem.h"
#include "Scene/Systems/Physics/PhysicsCollisionSystem.h"
#include "ProceduralPlanet.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Components/LineComponent.h"
#include "Components/ProceduralSphereMeshComponent.h"
#include "Components/PhysicsComponent.h"

SpaceExplorerScene::SpaceExplorerScene(const std::string& name)
    : Scene(name)
{
    MeshManager::Get().RegisterCreateAsset("spaceship", []
    {
        return MeshManager::LoadMesh("Models/Ship/space_ship.obj");
    });
    
    AddObject<GravitySystem>();
    AddObject<PhysicsCollisionSystem>();

    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({-1.0f, 120.0f, -35.0f});
    camera->cameraSpeed = 80.0f;
    cameraComp->Rotate(10.f, 180.0f);
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({20.1f, -30.1f, -40.0f});
    light->lightData.ambient = 0.6f;
    light->lightData.diffuse = 1.0f;
    
    auto proceduralPlanet = AddObject<ProceduralPlanet>();
    
    Gradient1D planetColourGradient;
    planetColourGradient.AddPoint(0.0f, glm::vec3(0.0f, 0.0f, 0.2f));
    planetColourGradient.AddPoint(0.4f, glm::vec3(0.0f, 0.3f, 0.6f));
    planetColourGradient.AddPoint(0.6f, glm::vec3(0.1f, 0.5f, 0.2f));
    planetColourGradient.AddPoint(0.8f, glm::vec3(0.5f, 0.4f, 0.2f));
    planetColourGradient.AddPoint(0.95f, glm::vec3(0.9f, 0.9f, 0.9f));
    planetColourGradient.AddPoint(1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    
    proceduralPlanet->SetPlanetColourGradient(planetColourGradient);
    
    auto planetMesh = proceduralPlanet->GetMeshComponent();
    
    planetMesh->SetNoiseSettings( 
    {
        {
            .strength = 0.11f,
            .frequency = 1.860f,
            .minValue = 0.0f,
            .maxValue = 1.0f,
            .offset = glm::vec3(0.0f, 0.0f, 0.0f)
        },
        {
            .strength = 0.110f,
            .frequency = 0.3f,
            .minValue = -1.0f,
            .maxValue = 1.00f,
            .offset = glm::vec3(0.0f, 0.480f, 0.0f)
        }
    });
    
    proceduralPlanet->SetScale({100.0f, 100.0f, 100.0f});
    
    auto phys = proceduralPlanet->GetPhysicsComponent();
    phys->physicsData.mass = 2230.0f;
    
    auto moon1 = AddObject<ProceduralPlanet>();
    
    Gradient1D moon1Gradient;
    moon1Gradient.AddPoint(0.0f, glm::vec3(0.05f, 0.05f, 0.05f));
    moon1Gradient.AddPoint(0.3f, glm::vec3(0.15f, 0.15f, 0.15f));
    moon1Gradient.AddPoint(0.55f, glm::vec3(0.25f, 0.24f, 0.23f));
    moon1Gradient.AddPoint(0.75f, glm::vec3(0.45f, 0.44f, 0.42f));
    moon1Gradient.AddPoint(0.9f, glm::vec3(0.65f, 0.64f, 0.62f));
    moon1Gradient.AddPoint(1.0f, glm::vec3(0.85f, 0.85f, 0.85f));
    
    moon1->SetPlanetColourGradient(moon1Gradient);
    
    planetMesh = moon1->GetMeshComponent();
    
    moon1->SetScale({20.0f, 20.0f, 20.0f});
    moon1->Move({800.0f, 50.0f, 50.0f});
    
    planetMesh->SetNoiseSettings(
    {
        {
            .strength = 0.05f,
            .frequency = 4.29f,
            .minValue = 0.0f,
            .maxValue = 1.f,
            .offset = glm::vec3(0.0f, 0.0f, 0.0f)
        },
        {
            .strength = 0.05f,
            .frequency = 2.62f,
            .minValue = -1.f,
            .maxValue = 1.f,
            .offset = glm::vec3(0.f, 0.43f, 0.f)
        }
    });
    
    phys = moon1->GetPhysicsComponent();
    
    phys->physicsData.mass = 10.0f;
    
    phys->physicsData.linearVelocity = {2.0f, 6.0f, 7.74f};
    auto line = moon1->AddComponent<LineComponent>();
    line->followParent = true;
    
    auto spaceShip = AddObject<SpaceShip>();
    spaceShip->SetPosition({0.0f, 120.0f, 0.0f});
    cameraComp->SetFollowTarget(spaceShip);
}
