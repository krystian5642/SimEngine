#pragma once

#include "Scene/Scene.h"
#include "../SpaceExplorer/ProceduralPlanet.h"

#define SCENE_NAME inline const std::string

class MeshEntity;
class SphereGrid;
class PhysicsComponent;
class Entity;

namespace SceneNames 
{
    SCENE_NAME ProjectileLauncher = "Projectile Launcher";
    SCENE_NAME BallCollision2D = "Ball Collision 2D";
    SCENE_NAME BallCollision3D = "Ball Collision 3D";
    SCENE_NAME SphereGrid = "Sphere Grid";
    SCENE_NAME GravityShip = "Gravity Ship";
    SCENE_NAME SolarSystem = "Solar System";
    SCENE_NAME PlanetAndMoons = "Planet and Moons";
    SCENE_NAME Materials = "Materials";
    SCENE_NAME RandomRigidBodies = "Random Rigid Bodies";
    SCENE_NAME ProceduralPlanet = "Procedural Planet";
    SCENE_NAME Instancing = "Instancing";
}

class ProjectileLauncherScene : public Scene
{
public:
    ProjectileLauncherScene(const std::string& name = SceneNames::ProjectileLauncher);
};

class BallCollision2DScene : public Scene
{
public:
    BallCollision2DScene(const std::string& name = SceneNames::BallCollision2D);
};

class BallCollision3DScene : public Scene
{
public:
    BallCollision3DScene(const std::string& name = SceneNames::BallCollision3D);
};

class SphereGridScene : public Scene
{
public:
    SphereGridScene(const std::string& name = SceneNames::SphereGrid);
    
    void DrawImGui() override;
    
private:
    SphereGrid* sphereGrid;
};

class SolarSystemScene : public Scene
{
public:
    SolarSystemScene(const std::string& name = SceneNames::SolarSystem);
};

class PlanetAndMoonsScene : public Scene
{
public:
    PlanetAndMoonsScene(const std::string& name = SceneNames::PlanetAndMoons);
};

class GravityShipScene : public Scene
{
public:
    GravityShipScene(const std::string& name = SceneNames::GravityShip);
};

class MaterialsScene : public Scene
{
public:
    MaterialsScene(const std::string& name = SceneNames::Materials);
};

class RandomRigidBodiesScene : public Scene
{
public:
    RandomRigidBodiesScene(const std::string& name = SceneNames::RandomRigidBodies);
    
    void Tick(float deltaTime) override;
    
    void DrawImGui() override;
    
private:
    void GenerateRandomRigidBody2D();
    void GenerateRandomRigidBody3D();
    
    void GenerateBall2D();
    void GenerateBall3D();
    
    void GenerateHammer();
    
    void ResetRigidBody();
    
    void ApplyTorque();
    
    bool showCenterOfMass{true};
    
    Entity* rigidBody{};
    MeshEntity* centerOfMassIndicator{};
    PhysicsComponent* phys{};
};

class ProceduralPlanetScene : public Scene
{
public:
    ProceduralPlanetScene(const std::string& name = SceneNames::ProceduralPlanet);
    
    void DrawImGui() override;
    
private:
    ProceduralPlanet* proceduralPlanet;
};

class InstancingScene : public Scene
{
public:
    InstancingScene(const std::string& name = SceneNames::Instancing);
};






