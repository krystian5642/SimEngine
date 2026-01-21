#include "Scenes.h"

#include "GravityShip.h"
#include "imgui.h"
#include "Core/MathUtils.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/CameraEntity.h"
#include "Scene/Systems/Physics/GravitySystem.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Scene/Systems/Physics/PhysicsBoundsSystem.h"
#include "Scene/Systems/Physics/PhysicsCollisionSystem.h"
#include "SphereGrid.h"

BallCollision2DScene::BallCollision2DScene(const std::string& name)
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
    light->lightData.ambient = 1.0f;
    
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            auto ball = AddObject<MeshEntity>();
            auto phys = ball->AddComponent<PhysicsComponent>();
            constexpr float velocityMagnitude = 7.0f;
            phys->runtimePhysicsData.linearVelocity = {MathUtils::randomNum(-velocityMagnitude, velocityMagnitude), MathUtils::randomNum(-velocityMagnitude, velocityMagnitude), 0.0f};
    
            phys->runtimePhysicsData.linearDamping = 1.0f;
            phys->runtimePhysicsData.angularDamping = 1.0f;
            
            ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
            ball->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
            ball->SetPosition({i*2, j*2, -2.0f});
            ball->SetRotation({90.0f, 0.0f, 0.0f});
            ball->SetScale({0.5f, 0.5f, 0.5f});
        }
    }
}

BallCollision3DScene::BallCollision3DScene(const std::string& name)
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
    light->lightData.ambient = 1.0f;
    
    glm::vec3 max{-INFINITY};
    glm::vec3 min{INFINITY};
    
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j <20; j++)
        {
            auto ball = AddObject<MeshEntity>();
            auto phys = ball->AddComponent<PhysicsComponent>();
            
            phys->runtimePhysicsData.linearDamping = 1.0f;
            phys->runtimePhysicsData.angularDamping = 1.0f;
       
            const float mass = MathUtils::randomNum(0.5f, 1.7f);
            phys->physicsData.mass = mass;
            
            constexpr float velocityMagnitude = 15.0f;
            
            phys->runtimePhysicsData.linearVelocity = {MathUtils::randomNum(-velocityMagnitude, velocityMagnitude)
                , MathUtils::randomNum(-velocityMagnitude, velocityMagnitude)
                , MathUtils::randomNum(-velocityMagnitude, velocityMagnitude)};
    
            ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
            ball->SetMaterial(MaterialManager::Get().GetAssetByName("emerald"));
            const glm::vec3 pos = {1.0f * static_cast<float>(i) + static_cast<float>(2 * i), 1.0f * static_cast<float>(j) + static_cast<float>(2 * j), -20.0f};
            ball->SetPosition(pos);
            ball->SetScale({mass, mass, mass});
            
            max = glm::max(max, pos);
            min = glm::min(min, pos);
        }
    } 
    
    boundsSystem->boundingBox.minBounds = min - glm::vec3{5.0f, 5.0f, 10.0f};
    boundsSystem->boundingBox.maxBounds = max + glm::vec3{5.0f, 5.0f, 10.0f};
}

SphereGridScene::SphereGridScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->lockRotation = true;
    cameraComp->SetPosition({0.0f, 0.0f, 20.0f});
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambient = 1.0f;
        
    sphereGrid = AddObject<SphereGrid>();
}

void SphereGridScene::DrawImGui() const
{
    Scene::DrawImGui();
    
    if (ImGui::Button("Apply Force"))
    {
        sphereGrid->CustomForce();
    }
}

GravityShipScene::GravityShipScene(const std::string& name)
    : Scene(name)
{
    AddObject<GravitySystem>();
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->lockRotation = true;
    cameraComp->SetPosition({2.8f, 0.0f, 13.0f});
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambient = 1.0f;
        
    auto sphere = AddObject<MeshEntity>();
    
    sphere->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    sphere->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    sphere->SetPosition({2.0f, 0.0f, -2.0f});
    sphere->SetScale({0.5f, 0.5f, 0.5f});
    
    sphere->AddComponent<PhysicsComponent>();
        
    AddObject<GravityShip>();
}
