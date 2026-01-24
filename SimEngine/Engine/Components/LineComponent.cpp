#include "LineComponent.h"

#include "Rendering/Renderer/Renderer.h"
#include "Scene/Objects/Entities/Entity.h"

LineComponent::LineComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    line = Renderer::CreateLineStatic();
}

void LineComponent::Tick(float deltaTime)
{
    RenderComponent::Tick(deltaTime);
    
    if (followParent)
    {
        auto actor = dynamic_cast<Entity*>(parent);
        AddPoint(actor->GetPosition());
    }
}

void LineComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
{
    if (visualPass)
    {
        line->Draw();
    }
}
