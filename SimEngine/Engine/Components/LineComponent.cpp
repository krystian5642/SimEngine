#include "LineComponent.h"

#include "Rendering/Renderer/Renderer.h"

LineComponent::LineComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    line = Renderer::CreateLineStatic();
}
    
void LineComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
{
    if (visualPass)
    {
        line->Draw();
    }
}
