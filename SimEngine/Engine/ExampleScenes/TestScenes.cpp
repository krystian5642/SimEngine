#include "TestScenes.h"

#include "BallLauncher.h"
#include "imgui.h"
#include "Components/LineComponent.h"
#include "Core/App.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/Gravity/SimpleGravitySystem.h"
#include "Scene/Objects/Entities/CameraEntity.h"
#include "Scene/Objects/Entities/MeshEntity.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Collisions/PhysicsBoundsSystem.h"
#include "Physics/Collisions/PhysicsCollisionSystem.h"
#include "Physics/Gravity/GravitySystem.h"

FallingBallsScene::FallingBallsScene(const std::string& name)
    : Scene(name)
{
    AddObject<SimpleGravitySystem>();
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({0.0f, 6.0f, 19.0f});
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection(glm::vec3(0.0f, -0.707f, -0.707f));
    light->lightData.color = glm::vec3(1.0f);
    light->lightData.diffuseIntensity = 1.0;
    
    for (int i = -5; i < 5; i++)
    {
        auto sphere = AddObject<MeshEntity>("Sphere");
        sphere->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
        sphere->meshComponent->material = MaterialManager::Get().GetAssetByName("copper");
        sphere->SetPosition({3.0f * i, MathUtils::RandomNum(5.0f, 10.0f), MathUtils::RandomNum(-7.0f, 7.0f)});
        sphere->SetScale({MathUtils::RandomScalarVec3(0.2f, 1.0f)});
        
        auto phys = sphere->AddComponent<PhysicsComponent>();
        phys->physicsData.restitution = MathUtils::RandomNum(0.4f, 0.9f);
    }
    
    auto plane = AddObject<MeshEntity>("Plane");
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("concrete");
    plane->SetPosition({0.0f, -1.0f, 0.0f});
    plane->SetScale({20.0f, 1.0f, 20.0f});
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f};   
}

GravityScene::GravityScene(const std::string& name)
    : Scene(name)
{
    AddObject<GravitySystem>();
    
    auto camera = AddObject<CameraEntity>("Camera");
    camera->cameraSpeed = 60.0f;
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 6.0f, 60.0f});
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection(glm::vec3(0.0f, -0.707f, -0.707f));
    light->lightData.color = glm::vec3(1.0f);
    light->lightData.diffuseIntensity = 1.0;
    
    auto mainPlanet = AddObject<MeshEntity>("Sphere");
    mainPlanet->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    mainPlanet->meshComponent->material = MaterialManager::Get().GetAssetByName("copper");
    mainPlanet->SetScale({10.0f, 10.0f, 10.0f});
    
    auto line = mainPlanet->AddComponent<LineComponent>();
    line->followParent = true;
    
    auto phys = mainPlanet->AddComponent<PhysicsComponent>();
    phys->physicsData.mass = 3000.0f;
    
    for (int i = 0; i < 500; i++)
    {
        auto sphere = AddObject<MeshEntity>("Sphere");
        sphere->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
        sphere->meshComponent->material = MaterialManager::Get().GetAssetByName("copper");
        sphere->SetPosition({120.0f, MathUtils::RandomNum(-3.0f, 3.0f), -10.0f + 0.3f * i});
        sphere->SetScale(MathUtils::RandomScalarVec3(0.4f, 1.0f));
        
        /*line = sphere->AddComponent<LineComponent>();
        line->followParent = true;
        line->line->maxLength = 200.0f;*/
        
        phys = sphere->AddComponent<PhysicsComponent>();
        phys->physicsData.mass = 0.1f;
        phys->physicsData.linearVelocity = {0.0f, MathUtils::RandomNum(1.0f, 1.1f), MathUtils::RandomNum(-25.0f, -20.0f)};
    }
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f};   
}

BallCollisions2DScene::BallCollisions2DScene(const std::string& name)
    : Scene(name)
{
    AddObject<PhysicsCollisionSystem>();
    AddObject<PhysicsBoundsSystem>();
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->lockRotation = true;
    cameraComp->SetPosition({14.0f, 11.0f, 40.0f});
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambientIntensity = 1.0f;
    
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            auto ball = AddObject<MeshEntity>();
            auto phys = ball->AddComponent<PhysicsComponent>();
            constexpr float velocityMagnitude = 7.0f;
            phys->physicsData.linearVelocity = {MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude), MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude), 0.0f};
            
            ball->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
            ball->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
            
            ball->Move({i*2, j*2, -2.0f});
            ball->SetRotation({90.0f, 0.0f, 0.0f});
            ball->SetScale({0.5f, 0.5f, 0.5f});
        }
    }
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

BallCollisions3DScene::BallCollisions3DScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({32.0f, 26.0f, 69.0f});
    
    AddObject<PhysicsCollisionSystem>();
    auto boundsSystem = AddObject<PhysicsBoundsSystem>();
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -60.0f});
    light->lightData.ambientIntensity = 1.0f;
    
    glm::vec3 max{-INFINITY};
    glm::vec3 min{INFINITY};
    
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j <20; j++)
        {
            auto ball = AddObject<MeshEntity>();
            auto phys = ball->AddComponent<PhysicsComponent>();
            
            const float mass = MathUtils::RandomNum(0.5f, 1.7f);
            phys->physicsData.mass = mass;
            
            constexpr float velocityMagnitude = 15.0f;
            
            phys->physicsData.linearVelocity = {MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude)
                , MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude)
                , MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude)};
            
            ball->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
            ball->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
            
            const glm::vec3 pos = {1.0f * static_cast<float>(i) + static_cast<float>(2 * i), 1.0f * static_cast<float>(j) + static_cast<float>(2 * j), -20.0f};
            ball->Move(pos);
            ball->SetScale({mass, mass, mass});
            
            max = glm::max(max, pos);
            min = glm::min(min, pos);
        }
    } 
    
    boundsSystem->boundingBox.minBounds = min - glm::vec3{5.0f, 5.0f, 10.0f};
    boundsSystem->boundingBox.maxBounds = max + glm::vec3{5.0f, 5.0f, 10.0f};
}

BallLauncherScene::BallLauncherScene(const std::string& name)
    : Scene(name)
{
    constexpr float groundLevel = -2.5f;
    
    AddObject<PhysicsCollisionSystem>();
    auto simpleGrav = AddObject<SimpleGravitySystem>();
    simpleGrav->groundLevel = groundLevel;
    
    AddObject<BallLauncher>();
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({37.0f, 14.0f, -12.0f});
    camera->SetRotation(-31.0f, 270.0f);
    
    auto plane = AddObject<MeshEntity>();
    
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    plane->SetScale({80.0f, 1.0f, 80.0f});
    plane->Move({0.0f, groundLevel, 0.0f});
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

CylindricalCoordinateScene::CylindricalCoordinateScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({14.0f, 4.0f, 0.0f});
    camera->SetRotation(-22.0f, 270.0f);
    
    auto plane = AddObject<MeshEntity>();
    
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    plane->SetScale({80.0f, 1.0f, 80.0f});
    plane->Move({0.0f, -2.0f, 0.0f});
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    ball = AddObject<MeshEntity>("Center Object");
    ball->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
    auto lineComponent = ball->AddComponent<LineComponent>();
    lineComponent->followParent = true;
    lineComponent->line->thickness = 3.0f;
    
    ball->SetCoordinateSystemType(CoordinateSystemType::Cylindrical);
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void CylindricalCoordinateScene::DrawImGui()
{
    Scene::DrawImGui();
    
    auto position = ball->GetPosition();
    
    ImGui::SeparatorText("Position (cylindrical)");

    bool changed = false;
    changed |= ImGui::DragFloat("Radius", &position.x, 0.05f, 0.0f);
    changed |= ImGui::DragFloat("Angle", &position.y, 0.05f, 0.0f, glm::two_pi<float>());
    changed |= ImGui::DragFloat("Height", &position.z, 0.05f);
        
    if (changed)
    {
        ball->SetPosition(position);
    }
}

SphericalCoordinateScene::SphericalCoordinateScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({14.0f, 4.0f, 0.0f});
    camera->SetRotation(-22.0f, 270.0f);
    
    auto plane = AddObject<MeshEntity>();
    
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    plane->SetScale({80.0f, 1.0f, 80.0f});
    plane->Move({0.0f, -2.0f, 0.0f});
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    ball = AddObject<MeshEntity>("Center Object");
    ball->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
    auto lineComponent = ball->AddComponent<LineComponent>();
    lineComponent->followParent = true;
    lineComponent->line->thickness = 3.0f;
    
    ball->SetCoordinateSystemType(CoordinateSystemType::Spherical);
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void SphericalCoordinateScene::DrawImGui()
{
    Scene::DrawImGui();
    
    auto position = ball->GetPosition();
    
    ImGui::SeparatorText("Position (spherical)");

    bool changed = false;
    changed |= ImGui::DragFloat("Radial distance", &position.x, 0.05f, 0.0f);
    changed |= ImGui::DragFloat("Azimuthal angle", &position.y, 0.05f, 0.0f, glm::pi<float>());
    changed |= ImGui::DragFloat("Polar angle", &position.z, 0.05f, 0.0f, glm::two_pi<float>());
        
    if (changed)
    {
        ball->SetPosition(position);
    }
}

