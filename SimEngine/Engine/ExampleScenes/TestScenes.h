#pragma once

#include "Physics/Gravity/GravitySystem.h"
#include "Scene/Scene.h"

#define SCENE_NAME inline const std::string

namespace SceneNames
{
    SCENE_NAME FallingBalls = "Falling Balls";
    SCENE_NAME Gravity = "Gravity";
    SCENE_NAME BallCollisions2D = "Ball Collisions 2D";
    SCENE_NAME BallCollisions3D = "Ball Collisions 3D";
    SCENE_NAME BallLauncher = "Ball Launcher";
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
