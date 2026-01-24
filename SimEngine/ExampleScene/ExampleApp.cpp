#include "ExampleApp.h"

#include "Scenes.h"
#include "Scene/SceneManager.h"

#define REGISTER_SCENE(name) SceneManager::RegisterScene(SceneNames::name, [] -> std::unique_ptr<Scene> { return std::make_unique<name##Scene>(); })

ExampleApp::ExampleApp()
{
    SceneManager::SetDefaultScene(SceneNames::RandomRigidBodies);
    
    REGISTER_SCENE(BallLauncher);
    REGISTER_SCENE(GravityShip);
    REGISTER_SCENE(BallCollision2D);
    REGISTER_SCENE(BallCollision3D);
    REGISTER_SCENE(SphereGrid);
    REGISTER_SCENE(SolarSystem);
    REGISTER_SCENE(PlanetAndMoons);
    REGISTER_SCENE(Materials);
    REGISTER_SCENE(RandomRigidBodies);
}