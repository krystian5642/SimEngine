#pragma once

#include "Component.h"

class Shader;

class RenderComponent : public Component
{
public:
    RenderComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Init() override;
    void OnDestroy() override;

    virtual void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const = 0;
};
