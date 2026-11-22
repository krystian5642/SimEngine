#pragma once

#include "Core/Properties/ClassProperty.h"
#include "Scene/DefaultScene.h"

class Entity;
class MeshComponent;
class SimpleGravityComponent;
class PhysicsSpringComponent;
class CompoundRigidBody;

#define SCENE_NAME inline const std::string

namespace SceneNames
{
    SCENE_NAME RigidBodyTest = "Rigid Body Test";
    SCENE_NAME GravityAndPlanets = "Gravity And Planets";
    SCENE_NAME SpringPendulum = "Spring Pendulum";
    SCENE_NAME CoupledSprings = "Coupled Springs";
    SCENE_NAME RigidBodyTShape = "Rigid Body T Shape";
}

class RigidBodyTestScene : public DefaultScene
{
public:
    RigidBodyTestScene(const std::string& name = SceneNames::RigidBodyTest);
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
};

class RigidBodyTShapeScene : public DefaultScene
{
public:
    RigidBodyTShapeScene(const std::string& name = SceneNames::RigidBodyTShape);
};

