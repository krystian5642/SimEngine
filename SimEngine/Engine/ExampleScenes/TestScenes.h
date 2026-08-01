#pragma once

#include "Components/MeshComponent.h"
#include "Components/VectorVisualizerComponent.h"
#include "Physics/Gravity/GravitySystem.h"
#include "Scene/Scene.h"

#define SCENE_NAME inline const std::string

class SpringComponent;
class MeshEntity;

namespace SceneNames
{
    SCENE_NAME FallingBalls = "Falling Balls";
    SCENE_NAME Gravity = "Gravity";
    SCENE_NAME BallCollisions2D = "Ball Collisions 2D";
    SCENE_NAME BallCollisions3D = "Ball Collisions 3D";
    SCENE_NAME BallLauncher = "Ball Launcher";
    SCENE_NAME CylindricalCoordinate = "Cylindrical Coordinate";
    SCENE_NAME SphericalCoordinate = "Spherical Coordinate";
    SCENE_NAME ArrowTest = "Arrow Test";
    SCENE_NAME CoriolisEffect = "Coriolis Effect";
    SCENE_NAME SpringTest = "Spring Test";
    SCENE_NAME HarmonicOscillator = "Harmonic Oscillator";
}

class FallingBallsScene : public Scene
{
public:
    FallingBallsScene(const std::string& name = SceneNames::FallingBalls);
};

class GravityScene : public Scene
{
public:
    GravityScene(const std::string& name = SceneNames::Gravity);
};

class BallCollisions2DScene : public Scene
{
public:
    BallCollisions2DScene(const std::string& name = SceneNames::BallCollisions2D);
};

class BallCollisions3DScene : public Scene
{
public:
    BallCollisions3DScene(const std::string& name = SceneNames::BallCollisions3D);
};

class BallLauncherScene : public Scene
{
public:
    BallLauncherScene(const std::string& name = SceneNames::BallLauncher);
};

class CylindricalCoordinateScene : public Scene
{
public:
    CylindricalCoordinateScene(const std::string& name = SceneNames::CylindricalCoordinate);
    
    void DrawImGui() override;
    
private:
    MeshEntity* ball;
};

class SphericalCoordinateScene : public Scene
{
public:
    SphericalCoordinateScene(const std::string& name = SceneNames::SphericalCoordinate);
    
    void DrawImGui() override;
    
private:
    MeshEntity* ball;
};

class ArrowTestScene : public Scene
{
public:
    ArrowTestScene(const std::string& name = SceneNames::ArrowTest);
    
    void Tick(float deltaTime) override;
    
    void DrawImGui() override;
    
private:
    glm::vec3 testStart{};
    glm::vec3 testDirection{1.0f, 0.0f, 0.0f};
    
    VectorVisualizerComponent* visualizer;
};

class CoriolisEffectScene : public Scene
{
public:
    CoriolisEffectScene(const std::string& name = SceneNames::CoriolisEffect);
    
    void Tick(float deltaTime) override;
    
    void DrawImGui() override;
    
private:
    MeshEntity* ball;
    MeshEntity* cylinder;
    VectorVisualizerComponent* velocityVisualizer;
    VectorVisualizerComponent* coriolisForceVisualizer;
    VectorVisualizerComponent* angularVelocityVisualizer;
    PhysicsComponent* physicsComponent;
    
    glm::vec3 coriolisForce;
    
    std::vector<MeshEntity*> spawnedEntities;
    
    float cylinderYawSpeed{5.0f};
    float spawnInterval{0.05f};
    float timeSinceLastSpawn{0.0f};
};


class SpringTestScene : public Scene
{
public:
    SpringTestScene(const std::string& name = SceneNames::SpringTest);
    
    void DrawImGui() override;
    
private:
    SpringComponent* springComponent;
};

class HarmonicOscillatorScene : public Scene
{
public:
    HarmonicOscillatorScene(const std::string& name = SceneNames::HarmonicOscillator);
    
    void Tick(float deltaTime) override;
    
    void DrawImGui() override;
    
private:
    SpringComponent* spring;
    MeshComponent* sphere1;
    
    const glm::vec3 equilibriumPosition{0.0f, -2.0f, 0.0f};
    const glm::vec3 initialBallPosition{0.0f, -4.0f, 0.0f};
    const float k{10.0f};
    const float ballMass{5.0f};
    
    glm::vec3 currentBallPosition = initialBallPosition;
    glm::vec3 currentBallVelocity{0.0f};
    const glm::vec3 gravityForce{0.0f, -9.81f * ballMass, 0.0f};
};