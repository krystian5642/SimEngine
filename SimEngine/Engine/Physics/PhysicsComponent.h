#pragma once

#include "Components/Component.h"

class Entity;

class PhysicsComponent : public Component
{
public:
    using Component::Component;
    
    void Init() override;
    
protected:
    template<class T, class TSelf>
    void Register(this TSelf& self);
    
    template<class T, class TSelf>
    void Unregister(this TSelf& self);
    
    Entity* parentEntity;
};

template <class T, class TSelf>
void PhysicsComponent::Register(this TSelf& self)
{
    if (T* system = T::Get())
    {
        system->Register(&self);
    }
}

template <class T, class TSelf>
void PhysicsComponent::Unregister(this TSelf& self)
{
    if (T* system = T::Get())
    {
        system->Unregister(&self);
    }
}
