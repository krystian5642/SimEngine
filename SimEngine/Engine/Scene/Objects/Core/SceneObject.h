#pragma once

#include "ObjectBase.h"

class SceneObject;

struct SceneObjectHandle
{
    const Scene* scene{};
    size_t index{};
    unsigned int version{};
    
    SceneObject* Resolve() const;
};

class SceneObject : public ObjectBase
{
    friend class Scene;
public:
    SceneObject(ObjectBase* parent, Scene* scene, const std::string& name);

    virtual void Init() {}
    virtual void Start() {}
    virtual void Tick(float deltaTime) {}
    virtual void OnDestroy();
    
    void Destroy();
    
    const SceneObjectHandle& GetHandle() const { return handle; }
    
    bool tickWhenPaused = false;

protected:
    Scene* const scene;
    
private:
    SceneObjectHandle handle;
};
