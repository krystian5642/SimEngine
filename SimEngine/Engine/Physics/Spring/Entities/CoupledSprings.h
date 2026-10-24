#pragma once

#include "Scene/Objects/Entities/Entity.h"

class SimpleGravityComponent;

class CoupledSprings : public Entity
{
public:
    CoupledSprings(const SceneObjectParams& params);
    
    PROPERTY(IntProperty, springsNum, 3, 1, 20)
    
private:
    void RecreateSprings();
    void OnSpringsNumChanged(int newSpringsNum);
};
