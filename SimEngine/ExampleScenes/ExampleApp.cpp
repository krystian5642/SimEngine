#include "ExampleApp.h"

#include "Scene/SceneManager.h"
#include "SpaceExplorer/SpaceExplorerScene.h"
#include "TestScenes/TestScenes.h"

#define REGISTER_SCENE(name) SceneManager::RegisterScene(SceneNames::name, [] -> std::unique_ptr<Scene> { return std::make_unique<name##Scene>(); })

ExampleApp::ExampleApp()
{
    SceneManager::SetDefaultScene(SceneNames::SpecularLightTest);
    
    // test scenes
    REGISTER_SCENE(ProjectileLauncher);
    REGISTER_SCENE(GravityShip);
    REGISTER_SCENE(BallCollision2D);
    REGISTER_SCENE(BallCollision3D);
    REGISTER_SCENE(SphereGrid);
    REGISTER_SCENE(SolarSystem);
    REGISTER_SCENE(PlanetAndMoons);
    REGISTER_SCENE(Materials);
    REGISTER_SCENE(RandomRigidBodies);
    REGISTER_SCENE(ProceduralPlanet);
    REGISTER_SCENE(Instancing);
    REGISTER_SCENE(RotationTest);
    REGISTER_SCENE(Antialiasing);
    REGISTER_SCENE(MipMapping);
    REGISTER_SCENE(SpecularLightTest);
    
    // main scenes
    REGISTER_SCENE(SpaceExplorer);
}