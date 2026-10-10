#pragma once

#include "Components/SpringComponent.h"
#include "Core/Properties/ClassProperty.h"
#include "Scene/DefaultScene.h"
#include "Scene/Objects/Entities/Entity.h"

class SimpleGravityComponent;
class PhysicsSpringComponent;
class RigidBody;

#define SCENE_NAME inline const std::string

namespace SceneNames
{
    SCENE_NAME RigidBodyTest = "Rigid Body Test";
    SCENE_NAME GravityAndPlanets = "Gravity And Planets";
    SCENE_NAME Spring = "Spring";
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

class SpringScene : public DefaultScene
{
public:
    SpringScene(const std::string& name = SceneNames::Spring);
    
private:
    PhysicsSpringComponent* spring;
    SimpleGravityComponent* body;
    
    PROPERTY(FunctionProperty, applyForce)
};