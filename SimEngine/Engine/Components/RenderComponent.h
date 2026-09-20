#pragma once

#include "Component.h"

class RenderComponent : public Component
{
public:
    using Component::Component;
    
    void Init() override;
    void OnDestroy() override;

    virtual void Draw() const = 0;
    
    void DrawUI() override;
    
    bool visible{true};
};
