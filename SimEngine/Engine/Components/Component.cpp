#include "Component.h"

Component::Component(ObjectBase* parent, Scene* scene, const std::string& name)
    : SceneObject(parent, scene, name)
{
}