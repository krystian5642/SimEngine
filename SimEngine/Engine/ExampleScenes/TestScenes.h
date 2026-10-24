#pragma once

#include "Core/Properties/ClassProperty.h"
#include "Scene/DefaultScene.h"

class SimpleGravityComponent;
class PhysicsSpringComponent;
class RigidBody;

#define SCENE_NAME inline const std::string

namespace SceneNames
{
    SCENE_NAME RigidBodyTest = "Rigid Body Test";
    SCENE_NAME GravityAndPlanets = "Gravity And Planets";
    SCENE_NAME SpringPendulum = "Spring Pendulum";
    SCENE_NAME CoupledSprings = "Coupled Springs";
}

class RigidBodyTestScene : public DefaultScene
{
public:
    RigidBodyTestScene(const std::string& name = SceneNames::RigidBodyTest);

private:
    RigidBody* rigidBody;
    
    PROPERTY(FunctionProperty, applyTorque)
};

class GravityAndPlanetsScene : public DefaultScene
{
public:
    GravityAndPlanetsScene(const std::string& name = SceneNames::GravityAndPlanets);
};

class SpringPendulumScene : public DefaultScene
{
public:
    SpringPendulumScene(const std::string& name = SceneNames::SpringPendulum);
};

class CoupledSpringsScene : public DefaultScene
{
public:
    CoupledSpringsScene(const std::string& name = SceneNames::CoupledSprings);
    
private:
    PROPERTY(FunctionProperty, applyForce)
};

