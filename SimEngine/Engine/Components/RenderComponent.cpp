#include "RenderComponent.h"
#include "Scene/Scene.h"

RenderComponent::RenderComponent(const SceneObjectParams& params)
    : Component(params)
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