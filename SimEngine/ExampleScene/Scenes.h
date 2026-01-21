#pragma once

#include "Scene/Scene.h"

#define SCENE_NAME inline const std::string

class SphereGrid;

namespace SceneNames 
{
    SCENE_NAME BallCollision2D = "Ball Collision 2D";
    SCENE_NAME BallCollision3D = "Ball Collision 3D";
    SCENE_NAME SphereGrid = "Sphere Grid";
    SCENE_NAME GravityShip = "Gravity Ship";
}

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
    
    void DrawImGui() const override;
    
private:
    SphereGrid* sphereGrid;
};

class GravityShipScene : public Scene
{
public:
    GravityShipScene(const std::string& name = SceneNames::GravityShip);
};