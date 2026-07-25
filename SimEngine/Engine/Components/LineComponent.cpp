#include "LineComponent.h"

#include "Scene/Objects/Entities/Entity.h"

LineComponent::LineComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    line = std::make_unique<Line>();
}

void LineComponent::Tick(float deltaTime)
{
    RenderComponent::Tick(deltaTime);
    
    if (followParent)
    {
        auto actor = dynamic_cast<Entity*>(parent);
        line->AddPoint(actor->GetPosition(true));
    }
}

void LineComponent::Draw() const
{
    line->Draw();
}
