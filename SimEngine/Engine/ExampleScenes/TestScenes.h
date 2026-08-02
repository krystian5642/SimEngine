#pragma once

#include "Core/Plot.h"
#include "Scene/Scene.h"

#define SCENE_NAME inline const std::string

class VectorVisualizerComponent;
class PhysicsComponent;
class HarmonicOscillator_MassiveSpring;
class HarmonicOscillator_MasslessSpring;
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
    SCENE_NAME HarmonicOscillator_MasslessSpring = "Harmonic Oscillator Massless Spring";
    SCENE_NAME HarmonicOscillator_MassiveSpring = "Harmonic Oscillator Massive Spring";
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

class HarmonicOscillator_MasslessSpringScene : public Scene
{
public:
    HarmonicOscillator_MasslessSpringScene(const std::string& name = SceneNames::HarmonicOscillator_MasslessSpring);
    
    void Tick(float deltaTime) override;
    
    void DrawImGui() override;
    
private:
    HarmonicOscillator_MasslessSpring* harmonicOscillator;
    
    RuntimePlotData runtimePlotData;
};

class HarmonicOscillator_MassiveSpringScene : public Scene
{
public:
    HarmonicOscillator_MassiveSpringScene(const std::string& name = SceneNames::HarmonicOscillator_MassiveSpring);
    
    void Tick(float deltaTime) override;
    
    void DrawImGui() override;
    
private:
    HarmonicOscillator_MassiveSpring* harmonicOscillator;
    
    RuntimePlotData runtimePlotData;
};