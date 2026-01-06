#include "LineComponent.h"

#include "Rendering/Renderer/Renderer.h"

namespace SimEngine
{
    LineComponent::LineComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : RenderComponent(parent, scene, name)
    {
        line = Renderer::CreateLineStatic();
    }

    LineComponent::~LineComponent()
    {
    }

    void LineComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
    {
        if (visualPass)
        {
            line->Draw();
        }
    }
}
