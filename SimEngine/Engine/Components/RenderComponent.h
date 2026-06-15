#pragma once

#include "Component.h"

class RenderComponent : public Component
{
public:
    RenderComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Init() override;
    void OnDestroy() override;

    virtual void Draw() const = 0;
};
