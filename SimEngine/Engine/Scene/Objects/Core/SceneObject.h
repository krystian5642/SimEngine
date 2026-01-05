#pragma once

#include "ObjectBase.h"

namespace SimEngine
{
    struct SceneObjectHandle
    {
        const Scene* scene{};
        size_t index{};
        unsigned int version{};
        
        ObjectBase* Resolve() const;
    };
    
    class SceneObject : public ObjectBase
    {
        friend class Scene;
    public:
        SceneObject(ObjectBase* parent, Scene* scene, const std::string& name);
        ~SceneObject() override;
        
        virtual void Init() {}
        virtual void Start() {}
        virtual void Tick(float deltaTime) {}
        
        void Destroy();
        
        const SceneObjectHandle& GetHandle() const { return handle; }
        
    protected:
        virtual void OnDestroy();
        
        Scene* const scene;
        
    private:
        SceneObjectHandle handle;
    };
}