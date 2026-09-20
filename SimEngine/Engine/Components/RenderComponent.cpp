#include "RenderComponent.h"

#include "imgui.h"
#include "Scene/Scene.h"

void RenderComponent::Init()
{
    scene->RegisterRenderComponent(this);
}
    
void RenderComponent::OnDestroy()
{
    scene->UnregisterRenderComponent(this);
}

void RenderComponent::DrawUI()
{
    Component::DrawUI();
    
    ImGui::Checkbox("Visible", &visible);
}
