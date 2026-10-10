#include "TestScenes.h"

#include "Components/CameraComponent.h"
#include "Core/App.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Gravity/Components/GravityComponent.h"
#include "Physics/Gravity/Systems/GravitySystem.h"
#include "Physics/Gravity/Systems/SimpleGravitySystem.h"
#include "Physics/RigidBody/Entities/RigidBody.h"
#include "Scene/Objects/Entities/MeshEntity.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"
#include "Physics/Spring/PhysicsSpringComponent.h"
#include "Rendering/Core/Material.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"

RigidBodyTestScene::RigidBodyTestScene(const std::string& name)
    : DefaultScene(name)
{
    auto gravSys = AddObject<SimpleGravitySystem>("Simple Gravity System");
    gravSys->enableGravity = false;
    
    rigidBody = AddObject<RigidBody>("Rigid Body");
    rigidBody->shape->material->data.color.w = 0.6f;
    //rigidBody->rigidBodyComponent->forceAtLocationAffectsLinearMotion = false;
    
    camera->SetPosition({-1.5f, 4.7f, 8.7f});
    camera->SetRotation(-27.0f, 14.0f);
    
    applyTorque.Set([this]()
    {
        rigidBody->ApplyForceAtLocation({-0.8f, 0.2f, 0.1f}
            , {0.4f, 0.0f, 0.1f});
        
        const auto& properties 
            = rigidBody->shape->GetProperties();
        for (auto* property : properties)
        {
            property->readOnly = true;
        }
    });
    
    auto plane = AddObject<MeshEntity>();
    
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    plane->SetScale({7.0f, 1.0f, 7.0f});
    plane->Move({0.0f, -2.0f, 0.0f});
}

GravityAndPlanetsScene::GravityAndPlanetsScene(const std::string& name)
    : DefaultScene(name)
{
    App::Get().renderer.clearColor = {0.70f, 0.85f, 0.95f};
    
    physicsDeltaTime = 1.0f / 120.0f;
    
    camera->SetPosition({-35.0f, 25.0f, 40.0f});
    camera->SetRotation(-24.0f, 40.0f);
    
    AddObject<GravitySystem>("Gravity System");
    
    auto bigPlanet = AddObject<Entity>("Big Planet");
    bigPlanet->openUIByDefault = true;
    
    auto grav = bigPlanet->AddComponent<GravityComponent>();
    
    auto sphere = bigPlanet->AddComponent<SphereComponent>("Planet");
    sphere->openUIByDefault = true;
    sphere->radius = 3.0f;
    
    sphere->mass.maxValue = 10000000.0f;
    sphere->mass = 100000.0f;
    /*grav->canBeAttracted = false;
    grav->canMove = false;*/
    grav->canRotate = false;
    
    grav->UpdateShapeProperties(sphere);

    for (int i = 0; i < 350; i++)
    {
        auto moon = AddObject<Entity>();
        moon->drawUI = false;
        sphere = moon->AddComponent<SphereComponent>();
        sphere->mass.maxValue = 1.0f;
        sphere->mass = 1.3f;
        
        grav = moon->AddComponent<GravityComponent>();
        //grav->canAttract = false;
        grav->canRotate = false;
        
        grav->UpdateShapeProperties(sphere);
        
        moon->SetPosition({100.0f, 1.0f, -10.0f + 0.3f * i});
        moon->SetScale(MathUtils::RandomScalarVec3(0.4f, 1.0f));
        
        grav->SetLinearVelocity({0.0f, 1.05f,-30.0f});
    }
}

SpringScene::SpringScene(const std::string& name)
    : DefaultScene(name)
{
    applyForce.Set([this]()
    {
        body->ApplyForce({-4.0f, 0.0f, -2.0f}, true);
    });
    
    physicsDeltaTime = 1.0f / 120.0f;
    
    light->SetDirection({0.4f, 2.0f, -1.1f});
    
    camera->SetPosition({-2.3f, -1.6f, 21.f});
    camera->SetRotation(0.0f, 4.0f);
    
    auto gravSys = AddObject<SimpleGravitySystem>("Simple Gravity System");
    gravSys->enableGravity = true;
    
    auto springObj = AddObject<Entity>();
    springObj->openUIByDefault = true;
    
    spring = springObj->AddComponent<PhysicsSpringComponent>("Physics Spring Component");
    spring->radius = 0.4f;
    spring->coilsNum = 13.0f;
    spring->openUIByDefault = true;
    spring->GetSpringLine()->thickness = 2.0f;
    spring->GetSpringLine()->color = glm::vec4{1.0f, 0.9f, 0.3f, 1.0f};
    spring->GetSpringLine()->thickness = 3.0f;
    
    auto meshCube = springObj->AddComponent<MeshComponent>();
    meshCube->mesh = MeshManager::Get().GetAssetByName("box");
    meshCube->material = MaterialManager::Get().GetAssetByName("bronze");
    meshCube->Move(glm::vec3(0.0f, 0.0f, 0.0f));
    meshCube->SetScale(glm::vec3{3.5f, 0.6f, 3.5f});
    
    auto ball2 = AddObject<Entity>();
    auto sphere2 = ball2->AddComponent<SphereComponent>();
    body = ball2->AddComponent<SimpleGravityComponent>();
    body->UpdateShapeProperties(sphere2);
    body->enableGravity = true;
    
    ball2->SetPosition(spring->end);
    spring->AttachComponentToEnd(body);
    
    ball2->SetScale(glm::vec3{1.3});
}