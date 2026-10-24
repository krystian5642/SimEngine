#pragma once

#include "Scene/Objects/Entities/Entity.h"

class SimpleGravityComponent;
class PhysicsSpringComponent;

class SpringPendulum : public Entity
{
public:
    SpringPendulum(const SceneObjectParams& params);
};
