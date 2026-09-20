#pragma once

#include "Scene/Objects/Core/SceneObject.h"

class Component : public SceneObject
{
public:
    using SceneObject::SceneObject;
    
    virtual ~Component() = 0 {}
};
