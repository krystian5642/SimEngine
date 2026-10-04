#pragma once

#include "Component.h"
#include "Core/Properties/ClassProperty.h"

class RenderComponent : public Component
{
public:
    RenderComponent(const SceneObjectParams& params);
    
    void Init() override;
    void OnDestroy() override;

    virtual void Draw() const = 0;
    virtual bool IsTransparent() const = 0;
    
    PROPERTY(BoolProperty, visible, true)
};
