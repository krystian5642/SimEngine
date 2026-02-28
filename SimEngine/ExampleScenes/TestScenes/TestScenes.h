#pragma once

#include "Scene/Scene.h"
#include "../SpaceExplorer/ProceduralPlanet.h"

#define SCENE_NAME inline const std::string

class MeshComponent;
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
    SCENE_NAME RotationTest = "Rotation Test";
    SCENE_NAME Antialiasing = "Anti-Aliasing";
    SCENE_NAME MipMapping = "Mip Mapping";
    SCENE_NAME NormalMapTest = "Normal Map Test";
    SCENE_NAME Earth = "Earth";
    SCENE_NAME ParallaxMapping = "Parallax Mapping";
    SCENE_NAME PolygonRenderModesAndTesselation = "Polygon Render Modes And Tesselation";
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

class RotationTestScene : public Scene
{
public:
    RotationTestScene(const std::string& name = SceneNames::RotationTest);
};

class AntialiasingScene : public Scene
{
public:
    AntialiasingScene(const std::string& name = SceneNames::Antialiasing);
    
    void DrawImGui() override;
};

class MipMappingScene : public Scene
{
public:
    MipMappingScene(const std::string& name = SceneNames::MipMapping);
};

class NormalMapTestScene : public Scene
{
public:
    NormalMapTestScene(const std::string& name = SceneNames::NormalMapTest);
};

class EarthScene : public Scene
{
public:
    EarthScene(const std::string& name = SceneNames::Earth);
    
    void DrawImGui() override;
};

class ParallaxMappingScene : public Scene
{
public:
    ParallaxMappingScene(const std::string& name = SceneNames::ParallaxMapping);
    
    void DrawImGui() override;
};

class PolygonRenderModesAndTesselationScene : public Scene
{
public:
    PolygonRenderModesAndTesselationScene(const std::string& name = SceneNames::PolygonRenderModesAndTesselation);
    
    void DrawImGui() override;
 
private:
    MeshComponent* planeMesh;
};






