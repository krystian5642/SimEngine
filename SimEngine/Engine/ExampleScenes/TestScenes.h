#pragma once

#include "Core/Plot.h"
#include "Scene/Scene.h"
 
#define SCENE_NAME inline const std::string

class Swing;
class SegmentComponent;
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
    SCENE_NAME SegmentComponentTest = "Segment Component Test";
    SCENE_NAME SwingTest = "Swing Test";
    SCENE_NAME ForcesTest = "Forces Test";
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
    
    void DrawUI() override;
    
private:
    MeshEntity* ball;
};

class SphericalCoordinateScene : public Scene
{
public:
    SphericalCoordinateScene(const std::string& name = SceneNames::SphericalCoordinate);
    
    void DrawUI() override;
    
private:
    MeshEntity* ball;
};

class ArrowTestScene : public Scene
{
public:
    ArrowTestScene(const std::string& name = SceneNames::ArrowTest);

    void DrawUI() override;
    
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
    
    void DrawUI() override;
    
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
    
    void DrawUI() override;
    
private:
    SpringComponent* springComponent;
};

class HarmonicOscillator_MasslessSpringScene : public Scene
{
public:
    HarmonicOscillator_MasslessSpringScene(const std::string& name = SceneNames::HarmonicOscillator_MasslessSpring);
    
    void Tick(float deltaTime) override;
    
    void DrawUI() override;
    
private:
    HarmonicOscillator_MasslessSpring* harmonicOscillator;
    
    RuntimePlotData runtimePlotData;
};

class HarmonicOscillator_MassiveSpringScene : public Scene
{
public:
    HarmonicOscillator_MassiveSpringScene(const std::string& name = SceneNames::HarmonicOscillator_MassiveSpring);
    
    void Tick(float deltaTime) override;
    
    void DrawUI() override;
    
private:
    HarmonicOscillator_MassiveSpring* harmonicOscillator;
    
    RuntimePlotData runtimePlotData;
};

class SegmentComponentTestScene : public Scene
{
public:
    SegmentComponentTestScene(const std::string& name = SceneNames::SegmentComponentTest);
    
    void DrawUI() override;
    
private:
    glm::vec3 start{};
    glm::vec3 end{1.0f, 0.0f, 0.0f};
    
    SegmentComponent* segment;
    MeshEntity* ball1;
    MeshEntity* ball2;
};

class SwingTestScene : public Scene
{
public:
    SwingTestScene(const std::string& name = SceneNames::SwingTest);

private:
    Swing* swing;
};

class ForcesTestScene : public Scene
{
public:
    ForcesTestScene(const std::string& name = SceneNames::ForcesTest);

    void Tick(float deltaTime) override;
    
    void DrawUI() override;
    
private:
    MeshEntity* ball1;
    MeshEntity* ball2;
    
    MeshEntity* ball3;
    
    VectorVisualizerComponent* F1Visualizer;
    VectorVisualizerComponent* F2Visualizer;
    
    VectorVisualizerComponent* V1xVisualizer;
    VectorVisualizerComponent* V1yVisualizer;
    VectorVisualizerComponent* V1Visualizer;
    
    VectorVisualizerComponent* V2xVisualizer;
    VectorVisualizerComponent* V2yVisualizer;
    VectorVisualizerComponent* V2Visualizer;
    
    CameraComponent* camera;
    
    SegmentComponent* segment;
    
    glm::vec3 V1{0.0f};
    float mass1{1.0f};
    
    glm::vec3 V2{0.0f};
    float mass2{2.0f};
    
    glm::vec3 impulseVelocityChange{0.0f, 5.0f, 0.0f};
    
    glm::vec3 prevCenterOfMassPos{0.0f};
};