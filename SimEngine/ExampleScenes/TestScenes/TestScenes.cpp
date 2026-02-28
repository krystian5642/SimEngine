#include "TestScenes.h"

#include "ProjectileLauncher.h"
#include "CelestialObject.h"
#include "GravityShip.h"
#include "imgui.h"
#include "Core/MathUtils.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/CameraEntity.h"
#include "Scene/Systems/Physics/GravitySystem.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Scene/Objects/Lighting/PointLightObject.h"
#include "Scene/Systems/Physics/PhysicsBoundsSystem.h"
#include "Scene/Systems/Physics/PhysicsCollisionSystem.h"
#include "SphereGrid.h"
#include "Components/InstancedMeshComponent.h"
#include "Components/LineComponent.h"
#include "Components/PhysicsComponent.h"
#include "Managers/TextureManager.h"
#include "Components/ProceduralSphereMeshComponent.h"
#include "Components/RotatingComponent.h"
#include "Rendering/Core/InstancedMesh.h"
#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Core/Texture.h"
#include "Scene/Systems/Physics/SimpleGravitySystem.h"
#include "Rendering/Core/Shader.h"
#include "Rendering/Core/Mesh.h"

ProjectileLauncherScene::ProjectileLauncherScene(const std::string& name)
    : Scene(name)
{
    const std::vector<std::string> skyboxFaces = {
        "Textures/Skybox/right.jpg",
        "Textures/Skybox/left.jpg",
        "Textures/Skybox/top.jpg",
        "Textures/Skybox/bottom.jpg",
        "Textures/Skybox/front.jpg",
        "Textures/Skybox/back.jpg"
    };
    
    AddObject<SimpleGravitySystem>();
    
    SetSkybox(Skybox::CreateSkybox(skyboxFaces));
    
    AddObject<ProjectileLauncher>();
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({14.0f, 4.0f, 25.0f});
    
    auto plane = AddObject<MeshEntity>();
    plane->SetMesh(MeshManager::Get().GetAssetByName("plane"));
    plane->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    plane->SetScale({400.0f, 1.0f, 400.0f});
    plane->Move({0.0f, -2.5f, 0.0f});
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambient = 0.5f;
    light->lightData.diffuse = 0.8f;
}

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
            phys->physicsData.linearVelocity = {MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude), MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude), 0.0f};
    
            phys->physicsData.linearDamping = 1.0f;
            phys->physicsData.angularDamping = 1.0f;
            
            ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
            ball->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
            ball->Move({i*2, j*2, -2.0f});
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
            
            phys->physicsData.linearDamping = 1.0f;
            phys->physicsData.angularDamping = 1.0f;
       
            const float mass = MathUtils::RandomNum(0.5f, 1.7f);
            phys->physicsData.mass = mass;
            
            constexpr float velocityMagnitude = 15.0f;
            
            phys->physicsData.linearVelocity = {MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude)
                , MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude)
                , MathUtils::RandomNum(-velocityMagnitude, velocityMagnitude)};
    
            ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
            ball->SetMaterial(MaterialManager::Get().GetAssetByName("emerald"));
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

SphereGridScene::SphereGridScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->lockRotation = true;
    cameraComp->SetPosition({0.0f, 0.0f, 80.0f});
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambient = 1.0f;
        
    sphereGrid = AddObject<SphereGrid>();
}

void SphereGridScene::DrawImGui()
{
    Scene::DrawImGui();
    
    if (ImGui::Button("Apply Force"))
    {
        sphereGrid->CustomForce();
    }
}

SolarSystemScene::SolarSystemScene(const std::string& name)
    : Scene(name)
{
    // sun
    TextureManager::Get().RegisterCreateAsset("sun", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_sun.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("sun", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
    
        data.ambient = glm::vec3{0.0f};
        data.specular = glm::vec3{0.0f};
        data.shininess = 0.0f;
        
        ShaderData shaderData;
        shaderData.fragShader = ShaderData::shadersFolder + "texture_only.frag";
        shaderData.vertShader = ShaderData::shadersFolder + "texture_only.vert";
        
        resources.shader = Shader::CreateShader(shaderData);
    
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("sun");
        return std::make_shared<Material>(resources); 
    });
    
    // mercury
    TextureManager::Get().RegisterCreateAsset("mercury", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_mercury.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("mercury", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {0.05f, 0.05f, 0.05f};
        data.diffuse = {0.3f, 0.28f, 0.25f};
        data.specular ={0.1f, 0.1f, 0.1f};
        data.shininess = 8.0f;  
        
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("mercury");
        return std::make_shared<Material>(resources); 
    });
    
    // venus
    TextureManager::Get().RegisterCreateAsset("venus", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_venus.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("venus", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.1f, 0.1f, 0.1f};
        data.diffuse  = {0.6f, 0.5f, 0.3f};
        data.specular = {0.05f, 0.05f, 0.05f};
        data.shininess = 4.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("venus");
        return std::make_shared<Material>(resources);
    });

    // earth
    TextureManager::Get().RegisterCreateAsset("earth", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_earth.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("earth", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.1f, 0.1f, 0.1f};
        data.diffuse  = {0.7f, 0.7f, 0.7f};
        data.specular = {0.2f, 0.2f, 0.2f};
        data.shininess = 16.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("earth");
        return std::make_shared<Material>(resources);
    });

    // mars
    TextureManager::Get().RegisterCreateAsset("mars", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_mars.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("mars", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.08f, 0.05f, 0.04f};
        data.diffuse  = {0.7f, 0.3f, 0.2f};
        data.specular = {0.1f, 0.05f, 0.05f};
        data.shininess = 8.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("mars");
        return std::make_shared<Material>(resources);
    });

    // jupiter
    TextureManager::Get().RegisterCreateAsset("jupiter", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_jupiter.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("jupiter", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.1f, 0.08f, 0.07f};
        data.diffuse  = {0.8f, 0.7f, 0.6f};
        data.specular = {0.05f, 0.05f, 0.05f};
        data.shininess = 2.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("jupiter");
        return std::make_shared<Material>(resources);
    });

    // saturn
    TextureManager::Get().RegisterCreateAsset("saturn", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_saturn.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("saturn", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.1f, 0.1f, 0.1f};
        data.diffuse  = {0.9f, 0.85f, 0.7f};
        data.specular = {0.05f, 0.05f, 0.05f};
        data.shininess = 2.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("saturn");
        return std::make_shared<Material>(resources);
    });

    // uranus
    TextureManager::Get().RegisterCreateAsset("uranus", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_uranus.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("uranus", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.05f, 0.08f, 0.1f};
        data.diffuse  = {0.5f, 0.8f, 0.9f};
        data.specular = {0.05f, 0.05f, 0.05f};
        data.shininess = 4.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("uranus");
        return std::make_shared<Material>(resources);
    });

    // neptune
    TextureManager::Get().RegisterCreateAsset("neptune", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_neptune.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("neptune", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.05f, 0.05f, 0.1f};
        data.diffuse  = {0.4f, 0.5f, 0.9f};
        data.specular = {0.1f, 0.1f, 0.15f};
        data.shininess = 6.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("neptune");
        return std::make_shared<Material>(resources);
    });
    
    const std::vector<std::string> skyboxFaces = {
        "Textures/Solar System/Skybox/right.png",
        "Textures/Solar System/Skybox/left.png",
        "Textures/Solar System/Skybox/top.png",
        "Textures/Solar System/Skybox/bottom.png",
        "Textures/Solar System/Skybox/front.png",
        "Textures/Solar System/Skybox/back.png"
    };
    
    SetSkybox(Skybox::CreateSkybox(skyboxFaces));
    
    AddObject<GravitySystem>();
    AddObject<PhysicsCollisionSystem>();
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({3.0f, 5.0f, 20.0f});
    camera->cameraSpeed = 20.0f;
        
    auto light = AddObject<PointLightObject>("Point Light");
    light->SetPosition({0.0f, 0.0f, -2.0f});
    light->lightData.ambient = 1.0f;
    light->lightData.diffuse = 2000.5f;
    
    auto sun = AddObject<CelestialObject>();
    sun->SetMaterial(MaterialManager::Get().GetAssetByName("sun"));
    sun->Move({0.0f, 0.0f, -2.0f});
    sun->SetScale({10.0f, 10.0f, 10.0f});
    
    auto phys = sun->GetPhysicsComponent();
    phys->physicsData.mass = 30.0f;
    phys->physicsData.angularVelocity = {0.0f, -80.0f, 0.0f};
    phys->physicsData.rotateWithCenterOfMass = false;
    phys->physicsData.physicsLinearConstraints = {true, true, true};
    
    auto mercury = AddObject<CelestialObject>();
    mercury->SetMaterial(MaterialManager::Get().GetAssetByName("mercury"));
    mercury->Move({-15.0f, 0.0f, -2.0f});
    mercury->SetScale({0.5f, 0.5f, 0.5f});
    
    phys = mercury->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -8.5f};
    
    auto venus = AddObject<CelestialObject>();
    venus->SetMaterial(MaterialManager::Get().GetAssetByName("venus"));
    venus->Move({-25.0f, 0.0f, -2.0f});
    venus->SetScale({1.5f, 1.5f, 1.5f});
    
    phys = venus->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -6.5f};
    
    auto earth = AddObject<CelestialObject>();
    earth->SetMaterial(MaterialManager::Get().GetAssetByName("earth"));
    earth->Move({-35.0f, 0.0f, -2.0f});
    earth->SetScale({1.4f, 1.4f, 1.4f});
    
    phys = earth->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -5.55f};
    
    auto mars = AddObject<CelestialObject>();
    mars->SetMaterial(MaterialManager::Get().GetAssetByName("mars"));
    mars->Move({-45.0f, 0.0f, -2.0f});
    mars->SetScale({0.7f, 0.7f, 0.7f});
    
    phys = mars->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -4.89f};
    
    auto jupiter = AddObject<CelestialObject>();
    jupiter->SetMaterial(MaterialManager::Get().GetAssetByName("jupiter"));
    jupiter->Move({-80.0f, 0.0f, -2.0f});
    jupiter->SetScale({4.8f, 4.8f, 4.8f});
    
    phys = jupiter->GetPhysicsComponent();
    phys->physicsData.mass = 3.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -2.5f};
    
    auto saturn = AddObject<CelestialObject>();
    saturn->SetMaterial(MaterialManager::Get().GetAssetByName("saturn"));
    saturn->Move({-100.0f, 0.0f, -2.0f});
    saturn->SetScale({3.0f, 3.0f, 3.0f});
    
    phys = saturn->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -3.25f};
    
    auto uranus = AddObject<CelestialObject>();
    uranus->SetMaterial(MaterialManager::Get().GetAssetByName("uranus"));
    uranus->Move({-120.0f, 0.0f, -2.0f});
    uranus->SetScale({2.5f, 2.5f, 2.5f});
    
    phys = uranus->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -3.0f};
    
    auto neptune = AddObject<CelestialObject>();
    neptune->SetMaterial(MaterialManager::Get().GetAssetByName("neptune"));
    neptune->Move({-140.0f, 0.0f, -2.0f});
    neptune->SetScale({2.0f, 2.0f, 2.0f});
    
    phys = neptune->GetPhysicsComponent();
    phys->physicsData.mass = 0.1f;
    phys->physicsData.linearVelocity = {0.0f, 0.0f, -2.7f};
}

PlanetAndMoonsScene::PlanetAndMoonsScene(const std::string& name)
    : Scene(name)
{
    // mercury
    TextureManager::Get().RegisterCreateAsset("mercury", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_mercury.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("mercury", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {0.05f, 0.05f, 0.05f};
        data.diffuse = {0.3f, 0.28f, 0.25f};
        data.specular ={0.1f, 0.1f, 0.1f};
        data.shininess = 8.0f;  
        
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("mercury");
        return std::make_shared<Material>(resources); 
    });
    
    // jupiter
    TextureManager::Get().RegisterCreateAsset("jupiter", []
    {
        return Texture::CreateTexture("Textures/Solar System/2k_jupiter.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("jupiter", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;

        data.ambient  = {0.1f, 0.08f, 0.07f};
        data.diffuse  = {0.8f, 0.7f, 0.6f};
        data.specular = {0.05f, 0.05f, 0.05f};
        data.shininess = 2.0f;

        resources.diffuseTexture = TextureManager::Get().GetAssetByName("jupiter");
        return std::make_shared<Material>(resources);
    });
    
    const std::vector<std::string> skyboxFaces = {
        "Textures/Solar System/Skybox/right.png",
        "Textures/Solar System/Skybox/left.png",
        "Textures/Solar System/Skybox/top.png",
        "Textures/Solar System/Skybox/bottom.png",
        "Textures/Solar System/Skybox/front.png",
        "Textures/Solar System/Skybox/back.png"
    };
    
    SetSkybox(Skybox::CreateSkybox(skyboxFaces));
    
    AddObject<GravitySystem>();
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({30.0f, 60.0f, 90.0f});
    camera->cameraSpeed = 50.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({-20.0f, 0.0f, 0.0f});
    light->lightData.ambient = 0.3f;
    light->lightData.diffuse = 0.7f;
    
    auto jupiter = AddObject<CelestialObject>();
    jupiter->SetMaterial(MaterialManager::Get().GetAssetByName("jupiter"));
    jupiter->Move({0.0f, 0.0f, -2.0f});
    jupiter->SetScale({10.0f, 10.0f, 10.0f});
    
    auto phys = jupiter->GetPhysicsComponent();
    phys->physicsData.mass = 3000.0f;
    phys->physicsData.angularVelocity = {0.0f, -80.0f, 0.0f};
    
    //jupiter->GetLineComponent()->followParent = false;
    
    for (int i = 0; i < 500; i++)
    {
        auto moon = AddObject<CelestialObject>();
        moon->SetMaterial(MaterialManager::Get().GetAssetByName("mercury"));
        moon->Move({110.0f, MathUtils::RandomNum(-3.0f, 3.0f), -10.0f + 0.3f * i});
        
        const float scale = MathUtils::RandomNum(0.4f, 1.0f);
        moon->SetScale({scale, scale, scale});
        
        phys = moon->GetPhysicsComponent();
        phys->physicsData.mass = 0.1f;
        phys->physicsData.linearVelocity = {0.0f, MathUtils::RandomNum(1.0f, 1.1f), MathUtils::RandomNum(-25.0f, -20.0f)};
        
        moon->GetLineComponent()->followParent = false;
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
    sphere->Move({2.0f, 0.0f, -2.0f});
    sphere->SetScale({0.5f, 0.5f, 0.5f});
    
    auto phys = sphere->AddComponent<PhysicsComponent>();
    phys->physicsData.linearDamping = 0.4f;    
    
    AddObject<GravityShip>();
}

MaterialsScene::MaterialsScene(const std::string& name)
    : Scene(name)
{
    MaterialManager::Get().RegisterCreateAsset("reflect", []
    {
        return ReflectMaterial::CreateReflectMaterial();
    });

    MaterialManager::Get().RegisterCreateAsset("refract", []
    {
        return RefractMaterial::CreateRefractMaterial();
    });
    
    const std::vector<std::string> skyboxFaces = {
        "Textures/Skybox/right.jpg",
        "Textures/Skybox/left.jpg",
        "Textures/Skybox/top.jpg",
        "Textures/Skybox/bottom.jpg",
        "Textures/Skybox/front.jpg",
        "Textures/Skybox/back.jpg"
    };

    SetSkybox(Skybox::CreateSkybox(skyboxFaces));
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 4.0f, 35.0f});
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambient = 1.0f;
    
    auto sphere = AddObject<MeshEntity>();
    sphere->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    sphere->SetMaterial(MaterialManager::Get().GetAssetByName("emerald"));
    sphere->Move({0.0f, 0.0f, 7.0f});
    sphere->SetScale({1.5f, 2.5f, 0.5f});

    auto reflectSphere = AddObject<MeshEntity>();
    reflectSphere->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    reflectSphere->SetMaterial(MaterialManager::Get().GetAssetByName("reflect"));
    reflectSphere->Move({-10.0f, 0.0f, 0.0f});
    reflectSphere->SetScale({4.5f, 4.5f, 0.5f});
    
    auto refractSphere = AddObject<MeshEntity>();
    refractSphere->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    refractSphere->SetMaterial(MaterialManager::Get().GetAssetByName("refract"));
    refractSphere->Move({10.0f, 0.0f, 0.0f});
    refractSphere->SetScale({4.5f, 4.5f, 0.5f});
}

RandomRigidBodiesScene::RandomRigidBodiesScene(const std::string& name)
    : Scene(name)
{
    AddObject<SimpleGravitySystem>();
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 4.0f, 35.0f});
    camera->cameraSpeed = 30.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambient = 1.0f;
    
    centerOfMassIndicator = AddObject<MeshEntity>();
    centerOfMassIndicator->SetMesh(MeshManager::Get().GetAssetByName("cube"));
    centerOfMassIndicator->SetMaterial(MaterialManager::Get().GetAssetByName("emerald"));
    centerOfMassIndicator->SetScale({1.0f, 1.0f, 20.0f});
    
    /*auto plane = AddObject<MeshEntity>();
    plane->SetMesh(MeshManager::Get().GetAssetByName("plane"));
    plane->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    plane->SetScale({100.0f, 1.0f, 100.0f});
    plane->Move({0.0f, -8.0f, 0.0f});*/
    
    GenerateRandomRigidBody3D();
}

void RandomRigidBodiesScene::Tick(float deltaTime)
{
    Scene::Tick(deltaTime);
    
    if (showCenterOfMass)
    {
        centerOfMassIndicator->Move(phys->physicsData.centerOfMass - centerOfMassIndicator->GetPosition());
    }
    else
    {
        centerOfMassIndicator->Move({-1000.0f, -1000.0f, -1000.0f});
    }
}

void RandomRigidBodiesScene::DrawImGui()
{
    Scene::DrawImGui();
    
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Scene Actions");
    
    if (ImGui::Button("Generate Random Rigid Body 2D"))
    {
        GenerateRandomRigidBody2D();
    }
    
    if (ImGui::Button("Generate Random Rigid Body 3D"))
    {
        GenerateRandomRigidBody3D();
    }
    
    if (ImGui::Button("Generate Ball 2D"))
    {
        GenerateBall2D();
    }
    
    if (ImGui::Button("Generate Ball 3D"))
    {
        GenerateBall3D();
    }
    
    if (ImGui::Button("Generate Hammer"))
    {
        GenerateHammer();
    }
    
    if (ImGui::Button("Apply Torque"))
    {
        ApplyTorque();
    }
    
    ImGui::Checkbox("Show Center of Mass", &showCenterOfMass);
    
    ImGui::Spacing();
}

void RandomRigidBodiesScene::GenerateRandomRigidBody2D()
{
    ResetRigidBody();
    
    int count = 0;
    for (int i = -7; i <= 7; i++)
    {
        for (int j = -7; j <= 7; j++)
        {
            const auto randomNumber = MathUtils::RandomNum(0.0f, 1.0f);
                
            if (randomNumber > 0.5f)
            {
                continue;
            }
                
            auto cube = rigidBody->AddComponent<MeshComponent>();
            cube->mesh = MeshManager::Get().GetAssetByName("sphere");
            cube->material = MaterialManager::Get().GetAssetByName("jade");
            cube->Move({i, j, 0.0f});
            
            count++;
        }
    }
    
    phys->physicsData.mass = static_cast<float>(count);
}

void RandomRigidBodiesScene::GenerateRandomRigidBody3D()
{
    ResetRigidBody();
    
    int count = 0;
    for (int i = -7; i <= 7; i++)
    {
        for (int j = -7; j <= 7; j++)
        {
            for (int k = -7; k <= 7; k++)
            {
                const auto randomNumber = MathUtils::RandomNum(0.0f, 1.0f);
                
                if (randomNumber > 0.2f)
                {
                    continue;
                }
                
                auto cube = rigidBody->AddComponent<MeshComponent>();
                cube->mesh = MeshManager::Get().GetAssetByName("sphere");
                cube->material = MaterialManager::Get().GetAssetByName("gold");
                cube->Move({i, j, k});
                
                count++;
            }
        }
    }
    
    phys->physicsData.mass = static_cast<float>(count);
}

void RandomRigidBodiesScene::GenerateBall2D()
{
    ResetRigidBody();
    
    int count = 0;
    for (int i = -7; i <= 7; i++)
    {
        for (int j = -7; j <= 7; j++)
        {
            if (i * i + j * j > 49)
            {
                continue;
            }
                
            auto cube = rigidBody->AddComponent<MeshComponent>();
            cube->mesh = MeshManager::Get().GetAssetByName("sphere");
            cube->material = MaterialManager::Get().GetAssetByName("silver");
            cube->Move({i, j, 0.0f});
            
            count++;
        }
    }
    
    phys->physicsData.mass = static_cast<float>(count);
}

void RandomRigidBodiesScene::GenerateBall3D()
{
    ResetRigidBody();
    
    int count = 0;
    for (int i = -7; i <= 7; i++)
    {
        for (int j = -7; j <= 7; j++)
        {
            for (int k = -7; k <= 7; k++)
            {
                if (i * i + j * j + k * k > 49)
                {
                    continue;
                }
                
                auto cube = rigidBody->AddComponent<MeshComponent>();
                cube->mesh = MeshManager::Get().GetAssetByName("sphere");
                cube->material = MaterialManager::Get().GetAssetByName("pearl");
                cube->Move({i, j, k});
                
                count++;
            }
        }
    }
    
    phys->physicsData.mass = static_cast<float>(count);
}

void RandomRigidBodiesScene::GenerateHammer()
{
    ResetRigidBody();
    
    int count = 0;
    for (int i = -7; i <= 7; i++)
    {
        for (int j = 18; j <= 22; j++)
        {
            for (int k = -4; k <= 4; k++)
            {
                auto cube = rigidBody->AddComponent<MeshComponent>();
                cube->mesh = MeshManager::Get().GetAssetByName("sphere");
                cube->material = MaterialManager::Get().GetAssetByName("bronze");
                cube->Move({i, j, k});
                
                count++;
            }
        }
    }
    
    for (int i = -2; i <= 2; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            auto cube = rigidBody->AddComponent<MeshComponent>();
            cube->mesh = MeshManager::Get().GetAssetByName("sphere");
            cube->material = MaterialManager::Get().GetAssetByName("concrete");
            cube->Move({i, j, 0.0f});
                
            count++;
        }
    }
    
    phys->physicsData.mass = static_cast<float>(count);
}

void RandomRigidBodiesScene::ResetRigidBody()
{
    if (rigidBody)
    {
        rigidBody->Destroy();
    }
    
    rigidBody = AddObject<Entity>();
    
    phys = rigidBody->AddComponent<PhysicsComponent>();
    phys->physicsData.rotateWithCenterOfMass = true;
    phys->physicsData.linearDamping = 0.7f;
    phys->physicsData.angularDamping = 0.7f;
}

void RandomRigidBodiesScene::ApplyTorque()
{
    phys = rigidBody->GetComponentByClass<PhysicsComponent>();
    
    const glm::vec3 force{MathUtils::RandomNum(-1000000.0f, 1000000.0f)
        , MathUtils::RandomNum(-1000000.0f / 2.0f, -1000000.0f / 2.0f)
        , MathUtils::RandomNum(-1000000.0f / 3.0f, -1000000.0f / 3.0f)};
    phys->ApplyTorque(force, {-7.0f, 7.0f, 0.0f});
}

ProceduralPlanetScene::ProceduralPlanetScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 4.0f, 35.0f});
    camera->cameraSpeed = 30.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({20.1f, -30.1f, -40.0f});
    light->lightData.ambient = 0.6f;
    light->lightData.diffuse = 1.0f;
    
    proceduralPlanet = AddObject<ProceduralPlanet>();
    proceduralPlanet->Move({0.0f, 0.0f, -1.0f});
    proceduralPlanet->SetScale({3.0f, 3.0f, 3.0f});
}

void ProceduralPlanetScene::DrawImGui()
{
    Scene::DrawImGui();
    
    bool changed = false;
    
    auto proceduralMesh = proceduralPlanet->GetMeshComponent();
    auto noiseSettingsList = proceduralMesh->GetNoiseSettings();
    
    ImGui::Text("Noise Settings");
    
    for (size_t i = 0; i < noiseSettingsList.size(); ++i)
    {
        auto& settings = noiseSettingsList[i];
        std::string label = "Layer " + std::to_string(i);
        
        if (ImGui::TreeNode(label.c_str()))
        {
            changed |= ImGui::DragFloat("Strength", &settings.strength, 0.01f, 0.0f, 100.0f);
            changed |= ImGui::DragFloat("Frequency", &settings.frequency, 0.01f, 0.0f, 100.0f);
            changed |= ImGui::DragFloat("Min Value", &settings.minValue, 0.01f, -1.0f, 1.0f);
            changed |= ImGui::DragFloat("Max Value", &settings.maxValue, 0.01f, -1.0f, 1.0f);
            changed |= ImGui::DragFloat3("Offset", glm::value_ptr(settings.offset), 0.01f);
            
            ImGui::TreePop();
        }
    }
    
    if (ImGui::Button("Add Layer"))
    {
        noiseSettingsList.push_back(NoiseSettings{});
        changed = true;
    }
    
    if (!noiseSettingsList.empty() && ImGui::Button("Remove Last Layer"))
    {
        noiseSettingsList.pop_back();
        changed = true;
    }
    
    if (changed)
    {
        proceduralMesh->SetNoiseSettings(noiseSettingsList);
    }
}

InstancingScene::InstancingScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 4.0f, 35.0f});
    camera->cameraSpeed = 30.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -40.0f});
    light->lightData.ambient = 1.0f;
    light->lightData.diffuse = 0.4f;
    
    // instance rendering
    auto obj = AddObject<Entity>();
    auto ins = obj->AddComponent<InstancedMeshComponent>();
    
    int X = 120;
    float spacing = 4.0f; 

    std::vector<Transform> transforms;
    transforms.reserve(X * X * X);

    for (int x = 0; x < X; ++x)
    {
        for (int y = 0; y < X; ++y)
        {
            for (int z = 0; z < X; ++z)
            {
                Transform t;

                t.position = {
                    x * spacing,
                    y * spacing,
                    z * spacing
                };

                transforms.push_back(t);
            }
        }
    }
    
    auto planet = MeshManager::Get().GetAssetByName("cube");
    
    ins->instancedMesh = InstancedMesh::CreateInstancedMesh(planet, transforms);
    ins->material = MaterialManager::Get().GetAssetByName("gold");
    
    /*auto obj = AddObject<Entity>();
    
    int X = 140;
    float spacing = 4.0f; 

    std::vector<Transform> transforms;
    transforms.reserve(X * X * X);

    for (int x = 0; x < X; ++x)
    {
        for (int y = 0; y < X; ++y)
        {
            for (int z = 0; z < X; ++z)
            {
                auto planet = MeshManager::Get().GetAssetByName("cube");
                
                auto ins = obj->AddComponent<MeshComponent>();
                ins->mesh = planet;
                ins->material = MaterialManager::Get().GetAssetByName("gold");
                
                Transform t;

                t.position = {
                    x * spacing,
                    y * spacing,
                    z * spacing
                };
                
                ins->SetPosition(t.position);
            }
        }
    }*/
}

RotationTestScene::RotationTestScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 0.0f, 80.0f});
    camera->cameraSpeed = 30.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -50.0f});
    light->lightData.ambient = 1.0f;
    light->lightData.diffuse = 0.4f;
    
    
    constexpr auto offset = 20.0f;
    for (int i = -15; i < 15; ++i)
    {
        for (int j = -2; j < 2; ++j)
        {
            for (int k = -2; k < 2; ++k)
            {
                auto cube = AddObject<MeshEntity>();
                cube->SetMesh(MeshManager::Get().GetAssetByName("cube"));
                cube->SetMaterial(MaterialManager::Get().GetAssetByName("gold"));
                cube->Move({i + offset, j + offset, k + offset});
                cube->SetScale({0.5f, 0.5f, 0.5f});
                
                cube->SetUseQuaternionsForRotation(true);
                
                auto line = cube->AddComponent<LineComponent>();
                line->SetThickness(2.0f);
                line->SetMaxLenght(250.0f);
                line->followParent = true;
                
                auto rot = cube->AddComponent<RotatingComponent>();
                rot->rotatingData.orbitAngularVelocity = {200.0f, 100.0f, 10.0f};
                rot->rotatingData.orbitCenter = {offset, offset, offset};
            }
        }
    }
    
    auto sphere = AddObject<MeshEntity>();
    sphere->SetUseQuaternionsForRotation(true);
    
    auto rot = sphere->AddComponent<RotatingComponent>();
    
    rot->rotatingData.localAngularVelocity = {50.0f, 50.0f, 50.0f};
    rot->rotatingData.orbitAngularVelocity = {50.0f, 50.0f, 50.0f};
    auto line = sphere->AddComponent<LineComponent>();
    line->followParent = true;
    line->SetThickness(2.0f);
    
    sphere->SetMesh(MeshManager::Get().GetAssetByName("cube"));
    sphere->SetMaterial(MaterialManager::Get().GetAssetByName("gold"));
    sphere->Move({0.0f, 0.0f, 7.0f});
    sphere->SetScale({7.5f, 0.5f, 0.5f});
}

AntialiasingScene::AntialiasingScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 0.0f, 80.0f});
    camera->cameraSpeed = 30.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -50.0f});
    light->lightData.ambient = 1.0f;
    light->lightData.diffuse = 0.4f;
    
    for (int i = -5; i < 5; ++i)
    {
        for (int j = -5; j < 5; ++j)
        {
            auto cube = AddObject<MeshEntity>();
            cube->SetMesh(MeshManager::Get().GetAssetByName("cube"));
            cube->SetMaterial(MaterialManager::Get().GetAssetByName("gold"));
            cube->Move({4*i, 4*j, 0.0f});
            cube->SetRotation({0.0f, 0.0f, 45.0f});
            
            auto rot = cube->AddComponent<RotatingComponent>();
            rot->rotatingData.localAngularVelocity = {0.0f, 0.0f, 30.0f};
            
            cube->SetUseQuaternionsForRotation(true);
        }
    }
}

void AntialiasingScene::DrawImGui()
{
    Scene::DrawImGui();

    auto method = static_cast<int>(Renderer::Get()->GetAntialiasingMethod());
    static const char* items[] = { "None", "FXAA", "MSAA" };

    ImGui::PushItemWidth(70);
    if (ImGui::Combo("AA Method", &method, items, IM_ARRAYSIZE(items)))
    {
        Renderer::Get()->SetAntialiasingMethod(static_cast<AntialiasingMethod>(method));
    }
    ImGui::PopItemWidth();

    if (method == static_cast<int>(AntialiasingMethod::FXAA))
    {
        bool changed = false;
        
        auto FXAA = Renderer::Get()->GetFXAASettings();
    
        changed |= ImGui::SliderFloat("Span Max",    &FXAA.FXAASpanMax,    1.0f, 100.0f);
        changed |= ImGui::SliderFloat("Reduce Min",  &FXAA.FXAAReduceMin,  0.0f, 100.0f);
        changed |= ImGui::SliderFloat("Reduce Mul",  &FXAA.FXAAReduceMul,  0.0f, 100.0f);
        
        if (changed)
        {
            Renderer::Get()->SetFXAASettings(FXAA);
        }   
    }
    else if (method == static_cast<int>(AntialiasingMethod::MSAA))
    {
        static const char* sampleItems[] = { "2", "4", "8", "16" };
        static int sampleValues[] = {  2,   4,   8,  16  };

        auto MSAA = Renderer::Get()->GetMSAASettings();
        
        int currentSamples = static_cast<int>(std::log2(MSAA.samples)) - 1;

        const auto maxSamples = static_cast<int>(Renderer::Get()->GetMaxSamples());
        auto availableCount = static_cast<int>(std::ranges::upper_bound(sampleValues, maxSamples) - std::begin(sampleValues));

        ImGui::PushItemWidth(70);
        if (ImGui::Combo("Samples", &currentSamples, sampleItems, availableCount))
        {
            MSAA.samples = sampleValues[currentSamples];
            Renderer::Get()->SetMSAASettings(MSAA);
        }
        ImGui::PopItemWidth();
    }
}

MipMappingScene::MipMappingScene(const std::string& name)
    : Scene(name)
{
    TextureManager::Get().RegisterCreateAsset("moon8k", []
    {
        return Texture::CreateTexture("Textures/8k_moon.jpg");
    });

    MaterialManager::Get().RegisterCreateAsset("moon8k", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {0.05f, 0.05f, 0.05f};
        data.specular = {0.1f, 0.1f, 0.1f};
        data.shininess = 8.0f;  
        
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("moon8k");
        return std::make_shared<Material>(resources); 
    });
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 2.0f, 0.0f});
    camera->cameraSpeed = 1.0f;
        
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, 0.1f, -50.0f});
    light->lightData.ambient = 1.0f;
    light->lightData.diffuse = 0.4f;
    
    auto plane = AddObject<MeshEntity>();
    plane->SetMesh(MeshManager::Get().GetAssetByName("plane"));
    plane->SetMaterial(MaterialManager::Get().GetAssetByName("moon8k"));
    plane->SetScale({2.0f, 0.0f, 2.0f});
}

NormalMapTestScene::NormalMapTestScene(const std::string& name)
    : Scene(name)
{
    TextureManager::Get().RegisterCreateAsset("bricks", []
    {
        return Texture::CreateTexture("Textures/Bricks/bricks.jpg", TextureFormat::sRGB);
    });

    TextureManager::Get().RegisterCreateAsset("bricks_normal", []
    {
        return Texture::CreateTexture("Textures/Bricks/bricks_normal.jpg", TextureFormat::RGB);
    });
    
    MaterialManager::Get().RegisterCreateAsset("bricks", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {0.05f, 0.05f, 0.05f};
        data.specular = {0.01f, 0.01f, 0.01f};
        data.shininess = 100.0f;  
        
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("bricks");
        resources.normalTexture = TextureManager::Get().GetAssetByName("bricks_normal");
        return std::make_shared<Material>(resources); 
    });
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({3.16f, 3.6f, 7.0f});
    cameraComp->SetRotation(-30.0f, 193.0f);
    camera->cameraSpeed = 5.0f;
    
    auto light = AddObject<PointLightObject>("Point Light");
    light->SetPosition({1.1f, 2.1f, 2.1f});
    light->lightData.ambient = 0.0f;
    light->lightData.diffuse = 8.2f;
    
    auto plane = AddObject<MeshEntity>();
    plane->SetMesh(MeshManager::Get().GetAssetByName("plane"));
    plane->SetMaterial(MaterialManager::Get().GetAssetByName("bricks"));
    plane->SetScale({4.0f, 1.0f, 4.0f});
}

EarthScene::EarthScene(const std::string& name)
    : Scene(name)
{
    TextureManager::Get().RegisterCreateAsset("earth", []
    {
        return Texture::CreateTexture("Textures/Earth/8k_earth.jpg", TextureFormat::sRGB);
    });

    TextureManager::Get().RegisterCreateAsset("earth_normal", []
    {
        return Texture::CreateTexture("Textures/Earth/8k_earth_normal_map.jpg", TextureFormat::RGB);
    });
    
    MaterialManager::Get().RegisterCreateAsset("earth", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {0.05f, 0.05f, 0.05f};
        data.specular = {0.01f, 0.01f, 0.01f};
        data.shininess = 100.0f;  
        
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("earth");
        resources.normalTexture = TextureManager::Get().GetAssetByName("earth_normal");
        
        return std::make_shared<Material>(resources); 
    });
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({3.16f, 3.6f, 7.0f});
    cameraComp->SetRotation(-30.0f, 193.0f);
    camera->cameraSpeed = 5.0f;
    
    auto light = AddObject<PointLightObject>("Point Light");
    light->SetPosition({0.1f, 0.1f, 60.1f});
    light->lightData.ambient = 80.1f;
    light->lightData.diffuse = 9000.2f;
    
    auto earth = AddObject<MeshEntity>();
    earth->SetMesh(MeshManager::Get().GetAssetByName("planet"));
    earth->SetMaterial(MaterialManager::Get().GetAssetByName("earth"));
    earth->SetRotation({0.0f, 210.0f, 0.0f});
    //auto rot = earth->AddComponent<RotatingComponent>();
    //rot->rotatingData.localAngularVelocity = {60.0f, 80.0f, 0.0f};
    //earth->SetUseQuaternionsForRotation(true);
}

void EarthScene::DrawImGui()
{
    Scene::DrawImGui();
    
    auto material = MaterialManager::Get().GetAssetByName("earth");
    ImGui::Checkbox("Use Normal Map", &material->GetResources().canUseNormalMap);
}

ParallaxMappingScene::ParallaxMappingScene(const std::string& name)
    : Scene(name)
{
    TextureManager::Get().RegisterCreateAsset("bricks", []
    {
        return Texture::CreateTexture("Textures/Bricks/bricks2.jpg", TextureFormat::sRGB);
    });

    TextureManager::Get().RegisterCreateAsset("bricks_normal", []
    {
        return Texture::CreateTexture("Textures/Bricks/bricks2_normal.jpg", TextureFormat::RGB);
    });
    
    TextureManager::Get().RegisterCreateAsset("bricks_disp", []
    {
        return Texture::CreateTexture("Textures/Bricks/bricks2_disp.jpg", TextureFormat::RGB);
    });
    
    MaterialManager::Get().RegisterCreateAsset("bricks", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {0.05f, 0.05f, 0.05f};
        data.specular = {0.01f, 0.01f, 0.01f};
        data.shininess = 100.0f;  
        
        resources.diffuseTexture = TextureManager::Get().GetAssetByName("bricks");
        resources.normalTexture = TextureManager::Get().GetAssetByName("bricks_normal");
        resources.displacementTexture = TextureManager::Get().GetAssetByName("bricks_disp");
        
        return std::make_shared<Material>(resources); 
    });
    
    auto camera = AddObject<CameraEntity>("Camera");
    auto cameraComp = camera->GetCameraComponent();
    cameraComp->SetAsActiveCamera();
    cameraComp->SetPosition({0.0f, 0.0f, 4.0f});
    camera->cameraSpeed = 1.0f;
    
    auto light = AddObject<PointLightObject>();
    light->SetPosition({3.1f, 5.7f, 3.1f});
    light->lightData.ambient = 0.9f;
    light->lightData.diffuse = 40.3f;
    
    auto plane = AddObject<MeshEntity>();
    plane->SetMesh(MeshManager::Get().GetAssetByName("plane"));
    plane->SetMaterial(MaterialManager::Get().GetAssetByName("bricks"));
   // plane->SetScale({4.0f, 1.0f, 4.0f});
    plane->SetRotation({90.0f, 0.0f, 0.0f});
}

void ParallaxMappingScene::DrawImGui()
{
    Scene::DrawImGui();
    
    auto& materialRes = MaterialManager::Get().GetAssetByName("bricks")->GetResources();
    ImGui::Checkbox("Use Normal Map", &materialRes.canUseNormalMap);
    
    ImGui::Text("Parallax Mapping");
    ImGui::Spacing();
    
    auto& parallaxMapping = materialRes.parallaxMappingData;
    ImGui::Checkbox("Enabled", &materialRes.parallaxMappingData.enabled);

    if (materialRes.parallaxMappingData.enabled)
    {
        ImGui::SliderFloat("Height Scale", &parallaxMapping.heightScale, 0.0f, 1.0f);
        ImGui::SliderInt("Min Layers", &parallaxMapping.minLayers, 1, 64);
        ImGui::SliderInt("Max Layers", &parallaxMapping.maxLayers, parallaxMapping.minLayers, 128);
        ImGui::Checkbox("Discard Fragments", &parallaxMapping.discardFragments);
    }
}
