#include "SceneObject.h"

#include "imgui.h"
#include "Scene/Scene.h"
#include "Core/Properties/ClassProperty.h"

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
    ImGui::PushID(this);
    for (auto* property : properties)
    {
        ImGui::BeginDisabled(property->readOnly);
        property->DrawUI();
        ImGui::EndDisabled();
    }
    ImGui::PopID();
}
