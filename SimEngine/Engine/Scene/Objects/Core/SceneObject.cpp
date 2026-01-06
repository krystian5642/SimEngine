#include "SceneObject.h"
#include "Scene/Scene.h"

namespace SimEngine
{
    ObjectBase* SceneObjectHandle::Resolve() const
    {
        return scene->GetObjectByHandle(*this);
    }
    
    SceneObject::SceneObject(ObjectBase* parent, Scene* scene, const std::string& name)
        : ObjectBase(parent, name)
        , scene(scene)
    {
        handle = scene->RegisterObject(this);
    }

    SceneObject::~SceneObject()
    {
    }

    void SceneObject::Destroy()
    {
        parent->DestroyChild(this);
    }

    void SceneObject::OnDestroy()
    {
        scene->UnregisterObject(this);
    }
}
