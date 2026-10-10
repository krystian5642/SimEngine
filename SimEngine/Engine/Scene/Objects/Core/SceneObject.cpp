#include "SceneObject.h"

#include "Scene/Scene.h"
#include "Core/Properties/ClassProperty.h"
#include "Core/UICommon/UICommon.h"

SceneObject* SceneObjectHandle::Resolve() const
{
    return scene->GetObjectByHandle(*this);
}

SceneObject::SceneObject(const SceneObjectParams& params)
    : ObjectBase(params.parent, params.name)
    , scene(params.scene)
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

void SceneObject::DrawUI()
{
    UICommon::DrawProperties(this, properties);
}
