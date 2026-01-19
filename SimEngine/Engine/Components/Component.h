#pragma once

#include "Scene/Objects/Core/SceneObject.h"

class Component : public SceneObject
{
public:
    Component(ObjectBase* parent, Scene* scene, const std::string& name);
    virtual ~Component() = 0 {}
};
