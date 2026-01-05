#include "Component.h"

namespace SimEngine
{
    Component::Component(ObjectBase* parent, Scene* scene, const std::string& name)
        : SceneObject(parent, scene, name)
    {
    }

    Component::~Component()
    {
    }
}
