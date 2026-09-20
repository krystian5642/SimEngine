#include "LineComponent.h"

#include "Scene/Objects/Entities/Entity.h"

LineComponent::LineComponent(const SceneObjectParams& params)
    : RenderComponent(params)
{
    line = std::make_unique<Line>();
}

void LineComponent::Tick(float deltaTime)
{
    RenderComponent::Tick(deltaTime);
    
    if (followParent)
    {
        auto actor = dynamic_cast<Entity*>(parent);
        line->AddPoint(actor->GetPosition());
    }
}

void LineComponent::Draw() const
{
    line->Draw();
}
