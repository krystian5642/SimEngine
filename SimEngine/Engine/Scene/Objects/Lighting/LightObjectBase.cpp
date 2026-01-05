
#include "LightObjectBase.h"

namespace SimEngine
{
    LightObjectBase::LightObjectBase(ObjectBase* parent, Scene* scene, const std::string& name)
        : SceneObject(parent, scene, name)
    {
    }
    
    LightObjectBase::~LightObjectBase()
    {
    }
}