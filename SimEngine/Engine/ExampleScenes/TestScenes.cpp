#include "TestScenes.h"

#include "BallLauncher.h"
#include "imgui.h"
#include "implot.h"
#include "Components/VectorVisualizerComponent.h"
#include "Components/LineComponent.h"
#include "Components/SegmentComponent.h"
#include "Components/SpringComponent.h"
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
#include "Physics/Objects/HarmonicOscillator_MasslessSpring.h"
#include "Physics/Objects/HarmonicOscillator_MassiveSpring.h"
#include "Physics/Objects/Swing.h"

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
    lineComponent->GetLine()->thickness = 3.0f;
    
    ball->SetCoordinateSystemType(CoordinateSystemType::Cylindrical);
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void CylindricalCoordinateScene::DrawUI()
{
    Scene::DrawUI();
    
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
    lineComponent->GetLine()->thickness = 3.0f;
    
    ball->SetCoordinateSystemType(CoordinateSystemType::Spherical);
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void SphericalCoordinateScene::DrawUI()
{
    Scene::DrawUI();
    
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

ArrowTestScene::ArrowTestScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({14.0f, 4.0f, 0.0f});
    camera->SetRotation(-22.0f, 270.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    auto ball = AddObject<MeshEntity>();
    ball->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
    ball->meshComponent->SetScale({0.2f, 0.2f, 0.2f});
    visualizer = ball->AddComponent<VectorVisualizerComponent>();
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void ArrowTestScene::DrawUI()
{
    Scene::DrawUI();
    
    ImGui::SeparatorText("Test Direction");

    bool changed = false;
    changed |= ImGui::DragFloat("X##Direction", &testDirection.x, 0.05f);
    changed |= ImGui::DragFloat("Y##Direction", &testDirection.y, 0.05f);
    changed |= ImGui::DragFloat("Z##Direction", &testDirection.z, 0.05f);

    ImGui::SeparatorText("Test Start");

    changed |= ImGui::DragFloat("X##Start", &testStart.x, 0.05f);
    changed |= ImGui::DragFloat("Y##Start", &testStart.y, 0.05f);
    changed |= ImGui::DragFloat("Z##Start", &testStart.z, 0.05f);

    if (changed)
    {
        visualizer->SetStart(testStart);
        visualizer->SetDirection(testDirection);
    }
}

CoriolisEffectScene::CoriolisEffectScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({18.0f, 12.0f, -18.0f});
    camera->SetRotation(-30.0f, -40.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    ball = AddObject<MeshEntity>();
    ball->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
    ball->SetPosition({15.0f, 4.0f, 0.0f});
    physicsComponent = ball->AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.mass = 0.3f;
    physicsComponent->physicsData.linearDamping = -1.0f;
    physicsComponent->physicsData.linearVelocity = {-3.0f, 0.0f, 0.0f};
    velocityVisualizer = ball->AddComponent<VectorVisualizerComponent>();
    velocityVisualizer->useParentLocationAsStart = true;
    velocityVisualizer->SetDirection(physicsComponent->physicsData.linearVelocity);
    
    coriolisForceVisualizer = ball->AddComponent<VectorVisualizerComponent>();
    coriolisForceVisualizer->useParentLocationAsStart = true;
    
    coriolisForce = 2 * physicsComponent->physicsData.mass * glm::cross(physicsComponent->physicsData.linearVelocity, {0.0f, glm::radians(cylinderYawSpeed), 0.0f});
    coriolisForceVisualizer->SetDirection(coriolisForce);
    coriolisForceVisualizer->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.9f);
    coriolisForceVisualizer->scaleLenghtFactor = 50.0f;
    
    cylinder = AddObject<MeshEntity>();
    cylinder->meshComponent->mesh = MeshManager::Get().GetAssetByName("cylinder");
    cylinder->meshComponent->material = MaterialManager::Get().GetAssetByName("turquoise");
    
    cylinder->SetPosition({0.0f, -4.0f, 0.0f});
    cylinder->SetScale({15.0f, 3.0f, 15.0f});
    angularVelocityVisualizer = cylinder->AddComponent<VectorVisualizerComponent>();
    angularVelocityVisualizer->SetStart(cylinder->GetPosition());
    angularVelocityVisualizer->SetDirection({0.0f, cylinderYawSpeed, 0.0f});
    
    angularVelocityVisualizer->color = glm::vec4(1.0f, 1.0f, 0.0f, 0.9f);
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void CoriolisEffectScene::Tick(float deltaTime)
{
    Scene::Tick(deltaTime);
    
    if (const auto isPaused = App::Get().isPaused)
    {
        return;
    }
    
    timeSinceLastSpawn += deltaTime;
    if (spawnInterval <= timeSinceLastSpawn && spawnedEntities.size() < 200)
    {
        auto newBall = AddObject<MeshEntity>();
        newBall->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
        newBall->meshComponent->material = MaterialManager::Get().GetAssetByName("obsidian");
    
        glm::vec3 position = ball->GetPosition();
        position.y = -4.0f;
    
        newBall->SetPosition(position);
        spawnedEntities.push_back(newBall);
        
        timeSinceLastSpawn = 0.0f;
    }
    
    for (auto* spawned : spawnedEntities)
    {
        glm::vec3 newPosition = spawned->GetPosition();
        
        auto modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix,  glm::radians(cylinderYawSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
        newPosition = glm::mat3(modelMatrix) * newPosition;
        
        spawned->SetPosition(newPosition);
    }
    
    velocityVisualizer->SetDirection(physicsComponent->physicsData.linearVelocity);
    
    coriolisForceVisualizer->SetDirection(coriolisForce);
    //physicsComponent->ApplyForce(coriolisForce);
    
    cylinder->Rotate({0.0f, cylinderYawSpeed * deltaTime, 0.0f});
}

void CoriolisEffectScene::DrawUI()
{
    ImGui::ColorButton("##yellow", ImVec4(1.0f, 1.0f, 0.0f, 0.9f), 
                        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
    ImGui::SameLine();
    ImGui::Text("Angular velocity");

    ImGui::ColorButton("##red", ImVec4(1.0f, 0.0f, 0.0f, 0.9f), 
                        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
    ImGui::SameLine();
    ImGui::Text("Velocity");

    ImGui::ColorButton("##white", ImVec4(1.0f, 1.0f, 1.0f, 0.9f), 
                        ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
    ImGui::SameLine();
    ImGui::Text("Coriolis force");
    
    Scene::DrawUI();
}

SpringTestScene::SpringTestScene(const std::string& name)
    : Scene(name)
{
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({-3.0f, 1.0f, 6.0f});
    camera->SetRotation(-14.0f, 158.0f);
    
    auto sprintObj = AddObject<Entity>();
    springComponent = sprintObj->AddComponent<SpringComponent>();
    auto springLine = springComponent->GetSpringLine();
    
    springLine->thickness = 4.0f;
    springLine->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

void SpringTestScene::DrawUI()
{
    Scene::DrawUI();
    
    int coilsEdit = springComponent->GetCoilsNum();
    if (ImGui::DragInt("Coils Num", &coilsEdit, 1, 1, 200))
    {
        springComponent->SetCoilsNum(coilsEdit);
    }
    
    glm::vec3 startEdit = springComponent->GetStart();
    if (ImGui::DragFloat3("Start", &startEdit.x, 0.05f))
    {
        springComponent->SetStart(startEdit);
    }
    
    glm::vec3 endEdit = springComponent->GetEnd();
    if (ImGui::DragFloat3("End", &endEdit.x, 0.05f))
    {
        springComponent->SetEnd(endEdit);
    }
    
    float radiusEdit = springComponent->GetRadius();
    if (ImGui::DragFloat("Radius", &radiusEdit, 0.02f, 0.01f, 50.0f))
    {
        springComponent->SetRadius(radiusEdit);
    }
    
    float deltaAngleEdit = springComponent->GetDeltaAngle();
    if (ImGui::DragFloat("Delta Angle", &deltaAngleEdit, 0.01f, 0.001f, 1.0f))
    {
        springComponent->SetDeltaAngle(deltaAngleEdit);
    }

}

HarmonicOscillator_MasslessSpringScene::HarmonicOscillator_MasslessSpringScene(const std::string& name)
    : Scene(name)
{
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({-1.0f, -2.5f, 24.0f});
    camera->SetRotation(0.0f, 180.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection(glm::vec3(0.0f, -0.707f, -0.707f));
    light->lightData.color = glm::vec3(1.0f);
    light->lightData.diffuseIntensity = 1.0;
    
    harmonicOscillator = AddObject<HarmonicOscillator_MasslessSpring>();
    
    runtimePlotData.plot.SetMaxPoints(500);
    runtimePlotData.plot.AddPoint(runtimePlotData.elapsedTime, 0.0f);
}

void HarmonicOscillator_MasslessSpringScene::Tick(float deltaTime)
{
    Scene::Tick(deltaTime);
    
    runtimePlotData.elapsedTime += deltaTime;
    runtimePlotData.timeSinceLastAddPoint += deltaTime;
    
    if (runtimePlotData.timeSinceLastAddPoint >= runtimePlotData.addPointInterval)
    {
        runtimePlotData.plot.AddPoint(runtimePlotData.elapsedTime, harmonicOscillator->GetOffset());
        runtimePlotData.timeSinceLastAddPoint = 0.0f;
    }
}

void HarmonicOscillator_MasslessSpringScene::DrawUI()
{
    Scene::DrawUI();
    
    auto& physicsData = harmonicOscillator->physicsData;
    
    ImGui::SliderFloat("Mass", &physicsData.mass, 0.1f, 50.0f, "%.2f kg");
    ImGui::SliderFloat("Spring length", &physicsData.springLength, 0.1f, 20.0f, "%.2f m");
    ImGui::SliderFloat("Spring constant (k)", &physicsData.k, 0.1f, 100.0f, "%.2f N/m");
    ImGui::SliderFloat("Gravity (g)", &physicsData.gravity, -20.0f, 20.0f, "%.2f m/s^2");
    
    ImPlot::BeginPlot("Harmonic Motion");
    
    ImPlot::SetupAxes("t", "x(t)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    ImPlot::PlotLine("x(t)"
        , runtimePlotData.plot.GetXValuesData()
        , runtimePlotData.plot.GetYValuesData()
        , runtimePlotData.plot.GetPointCount());
    
    ImPlot::EndPlot();
}

HarmonicOscillator_MassiveSpringScene::HarmonicOscillator_MassiveSpringScene(const std::string& name)
    : Scene(name)
{
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({-1.0f, -2.5f, 24.0f});
    camera->SetRotation(-7.0f, 180.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection(glm::vec3(0.0f, -0.707f, -0.707f));
    light->lightData.color = glm::vec3(1.0f);
    light->lightData.diffuseIntensity = 1.0;
    
    harmonicOscillator = AddObject<HarmonicOscillator_MassiveSpring>();
    
    runtimePlotData.plot.SetMaxPoints(500);
    runtimePlotData.plot.AddPoint(runtimePlotData.elapsedTime, 0.0f);
}

void HarmonicOscillator_MassiveSpringScene::Tick(float deltaTime)
{
    Scene::Tick(deltaTime);
    
    runtimePlotData.Tick(deltaTime, harmonicOscillator->GetOffset());
}

void HarmonicOscillator_MassiveSpringScene::DrawUI()
{
    Scene::DrawUI();
    
    auto physicsData = harmonicOscillator->GetPhysicsData();
    
    bool changed = false;

    changed |= ImGui::DragFloat("Mass", &physicsData.mass, 0.01f, 0.001f, 1000.0f, "%.3f kg");
    changed |= ImGui::DragFloat("Spring Length", &physicsData.springLength, 0.1f, 0.0f, 1000.0f, "%.2f m");
    changed |= ImGui::DragFloat("Spring Mass", &physicsData.springMass, 0.001f, 0.0f, 100.0f, "%.3f kg");
    changed |= ImGui::DragFloat("Spring constant (k)", &physicsData.k, 0.1f, 0.0f, 10000.0f, "%.2f N/m");
    changed |= ImGui::DragFloat("Gravity (g)", &physicsData.gravity, 0.01f, -100.0f, 100.0f, "%.2f m/s^2");
    changed |= ImGui::SliderInt("N (segments)", &physicsData.N, 1, 100);

    if (changed)
    {
        harmonicOscillator->SetPhysicsData(physicsData);
    }
    
    ImPlot::BeginPlot("Harmonic Motion");
    
    ImPlot::SetupAxes("t", "x(t)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    ImPlot::PlotLine("x(t)"
        , runtimePlotData.plot.GetXValuesData()
        , runtimePlotData.plot.GetYValuesData()
        , runtimePlotData.plot.GetPointCount());
    
    ImPlot::EndPlot();
}

SegmentComponentTestScene::SegmentComponentTestScene(const std::string& name)
    : Scene(name)
{
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({4.0f, 1.8f, 4.2f});
    camera->SetRotation(-13.0f, 223.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    auto entity = AddObject<Entity>();
    segment = entity->AddComponent<SegmentComponent>();
    
    ball1 = AddObject<MeshEntity>();
    ball1->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball1->meshComponent->material = MaterialManager::Get().GetAssetByName("gold");
    
    ball1->meshComponent->SetScale(glm::vec3{0.1f});
    
    ball2 = AddObject<MeshEntity>();
    ball2->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball2->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
    
    ball2->meshComponent->SetScale(glm::vec3{0.1f});
    
    ball1->SetPosition(start);
    ball2->SetPosition(end);
    
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
}

void SegmentComponentTestScene::DrawUI()
{
    Scene::DrawUI();
    
    ImGui::SeparatorText("Start");

    bool changed = false;
    changed |= ImGui::DragFloat("X##Start", &start.x, 0.05f);
    changed |= ImGui::DragFloat("Y##Start", &start.y, 0.05f);
    changed |= ImGui::DragFloat("Z##Start", &start.z, 0.05f);

    ImGui::SeparatorText("End");

    changed |= ImGui::DragFloat("X##End", &end.x, 0.05f);
    changed |= ImGui::DragFloat("Y##End", &end.y, 0.05f);
    changed |= ImGui::DragFloat("Z##End", &end.z, 0.05f);

    if (changed)
    {
        segment->SetStart(start);
        segment->SetEnd(end);
        
        ball1->SetPosition(start);
        ball2->SetPosition(end);
    }
}

SwingTestScene::SwingTestScene(const std::string& name)
    : Scene(name)
{
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
    
    auto camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({8.0f, 0.5f, 4.0f});
    camera->SetRotation(-10.5f, 253.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({30.1f, -60.0f, 20.1f});
    light->lightData.ambientIntensity = 0.7f;
    light->lightData.diffuseIntensity = 0.8f;
    
    swing = AddObject<Swing>();
}

ForcesTestScene::ForcesTestScene(const std::string& name)
    : Scene(name)
{
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
    
    camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    camera->SetPosition({24.0f, 2.1f, -2.5f});
    camera->SetRotation(-6.2f, 277.0f);
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
    
    ball1 = AddObject<MeshEntity>();
    ball1->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball1->meshComponent->material = MaterialManager::Get().GetAssetByName("gold");
    
    auto line1 = ball1->AddComponent<LineComponent>();
    line1->followParent = true;
    line1->GetLine()->thickness = 3.0f;
    line1->GetLine()->color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};
    
    segment = ball1->AddComponent<SegmentComponent>();
    segment->useParentLocationAsStart = true;
    
    F1Visualizer = ball1->AddComponent<VectorVisualizerComponent>();
    F1Visualizer->color = glm::vec4{1.0f, 1.0f, 0.0f, 0.9f};
    
    F1Visualizer->useParentLocationAsStart = true;
    
    V1xVisualizer = ball1->AddComponent<VectorVisualizerComponent>();
    V1xVisualizer->color = glm::vec4{0.5f, 1.0f, 0.0f, 0.9f};
    V1xVisualizer->useParentLocationAsStart = true;
    
    V1yVisualizer = ball1->AddComponent<VectorVisualizerComponent>();
    V1yVisualizer->color = glm::vec4{1.0f, 0.0f, 1.0f, 0.9f};
    V1yVisualizer->useParentLocationAsStart = true;

    V1Visualizer = ball1->AddComponent<VectorVisualizerComponent>();
    V1Visualizer->color = glm::vec4{1.0f, 0.0f, 0.0f, 0.9f};
    V1Visualizer->useParentLocationAsStart = true;
    
    ball2 = AddObject<MeshEntity>();
    ball2->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball2->meshComponent->material = MaterialManager::Get().GetAssetByName("emerald");
    
    auto line2 = ball2->AddComponent<LineComponent>();
    line2->followParent = true;
    line2->GetLine()->thickness = 3.0f;
    line2->GetLine()->color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    
    F2Visualizer = ball2->AddComponent<VectorVisualizerComponent>();
    F2Visualizer->color = glm::vec4{1.0f, 1.0f, 0.0f, 0.9f};
    F2Visualizer->useParentLocationAsStart = true;
    
    V2xVisualizer = ball2->AddComponent<VectorVisualizerComponent>();
    V2xVisualizer->color = glm::vec4{0.5f, 1.0f, 0.5f, 0.9f};
    V2xVisualizer->useParentLocationAsStart = true;
    
    V2yVisualizer = ball2->AddComponent<VectorVisualizerComponent>();
    V2yVisualizer->color = glm::vec4{1.0f, 0.0f, 1.0f, 0.9f};
    V2yVisualizer->useParentLocationAsStart = true;
    
    V2Visualizer = ball2->AddComponent<VectorVisualizerComponent>();
    V2Visualizer->color = glm::vec4{1.0f, 0.0f, 0.0f, 0.9f};
    V2Visualizer->useParentLocationAsStart = true;
    
    constexpr float scaleLenghtFactor{0.1f};
    F1Visualizer->scaleLenghtFactor = scaleLenghtFactor;
    F2Visualizer->scaleLenghtFactor = scaleLenghtFactor;
    
    constexpr float scaleLenghtFactor2{0.4f};
    V1xVisualizer->scaleLenghtFactor = scaleLenghtFactor2;
    V1yVisualizer->scaleLenghtFactor = scaleLenghtFactor2;
    V2xVisualizer->scaleLenghtFactor = scaleLenghtFactor2;
    V2yVisualizer->scaleLenghtFactor = scaleLenghtFactor2;
    V1Visualizer->scaleLenghtFactor = scaleLenghtFactor2;
    V2Visualizer->scaleLenghtFactor = scaleLenghtFactor2;
    
    F1Visualizer->scaleFactor = F2Visualizer->scaleFactor = 2.0f;
    
    ball1->SetPosition({0.0f, 0.0f, 3.0f});
    ball2->SetPosition({0.0f, 0.0f, -4.0f});
    
    ball1->SetScale(glm::vec3{0.3f});
    ball2->SetScale(glm::vec3{0.3f});
    
    ball3 = AddObject<MeshEntity>();
    ball3->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    ball3->meshComponent->material = MaterialManager::Get().GetAssetByName("brass");
    ball3->SetScale(glm::vec3{0.2f});
    ball3->SetPosition((ball1->GetPosition() * mass1 + ball2->GetPosition() * mass2) / (mass1 + mass2));
    auto line3 = ball3->AddComponent<LineComponent>();
    line3->followParent = true;
    line3->GetLine()->thickness = 2.0f;
    line3->GetLine()->color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
    
    prevCenterOfMassPos = (ball1->GetPosition() * mass1 + ball2->GetPosition() * mass2) / (mass1 + mass2);
}

void ForcesTestScene::Tick(float deltaTime)
{
    Scene::Tick(deltaTime);
    
    // calc forces
    
    const glm::vec3 dir = ball2->GetPosition() - ball1->GetPosition();
    const float r = glm::length(ball1->GetPosition() - ball2->GetPosition());
    const glm::vec3 dirNormalized = dir / r;
    const glm::vec3 relativeV = V2 - V1;
    const glm::vec3 vPerv = relativeV - glm::dot(dirNormalized, relativeV) * dirNormalized;
    
    const float mi = (mass1 * mass2) / (mass1 + mass2);
    
    const glm::vec3 F1 = (mi * (glm::dot(vPerv, vPerv)) / r) * dirNormalized;
    const glm::vec3 F2 = -F1;
    
    F1Visualizer->SetDirection(F1);
    F2Visualizer->SetDirection(F2);
    
    V1 += (F1 / mass1) * deltaTime;
    V2 += (F2 / mass2) * deltaTime;
    
    ball1->Move(V1 * deltaTime);
    ball2->Move(V2 * deltaTime);
     
    segment->SetEnd(ball2->GetPosition());
    
    const glm::vec3 centerOfMassPos = (ball1->GetPosition() * mass1 + ball2->GetPosition() * mass2) / (mass1 + mass2);
    ball3->SetPosition(centerOfMassPos);
    
    const glm::vec3& cameraPos = camera->GetPosition();
    camera->SetPosition({cameraPos.x, centerOfMassPos.y, cameraPos.z});
    
    const glm::vec3 centerOfMassVel = (centerOfMassPos - prevCenterOfMassPos) / deltaTime;
    V1xVisualizer->SetDirection(V1 - centerOfMassVel);
    V1yVisualizer->SetDirection(centerOfMassVel);
    V1Visualizer->SetDirection(V1);
    
    V2xVisualizer->SetDirection(V2 - centerOfMassVel);
    V2yVisualizer->SetDirection(centerOfMassVel);
    V2Visualizer->SetDirection(V2);
    
    prevCenterOfMassPos = centerOfMassPos;
}

void ForcesTestScene::DrawUI()
{
    Scene::DrawUI();
    
    if (ImGui::Button("Apply Impulse"))
    {
        V1 += impulseVelocityChange;
    }
    
    if (ImGui::Button("Stop center of mass"))
    {
        const glm::vec3 centerOfMassVel = impulseVelocityChange * (mass1 / (mass1 + mass2));
        V1 -= glm::vec3{centerOfMassVel};
        V2 -= glm::vec3{centerOfMassVel};
    }
}
