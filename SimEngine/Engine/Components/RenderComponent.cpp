#include "RenderComponent.h"

#include "Scene/Scene.h"

RenderComponent::RenderComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
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