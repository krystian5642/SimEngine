#include "RenderComponent.h"

#include "Scene/Scene.h"

namespace SimEngine
{
    RenderComponent::RenderComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : Component(parent, scene, name)
    {
    }

    RenderComponent::~RenderComponent()
    {
    }

    void RenderComponent::Init()
    {
        scene->RegisterRenderComponent(this);
    }

    void RenderComponent::OnDestroy()
    {
        scene->UnregisterRenderComponent(this);
    }
}
