#pragma once

namespace SimEngine
{
    class Scene;
    class ObjectBase;
    
    class ObjectBase
    {
    public:
        ObjectBase(ObjectBase* parent, const std::string& name);
        virtual ~ObjectBase() = 0;
        
        virtual void DestroyChild(ObjectBase* child) {}
        
        ObjectBase* GetParent() const { return parent; }
        void SetParent(ObjectBase* newParent) { parent = newParent; }
        
        const std::string& GetName() const { return name; }
        void SetName(std::string newName) { name = std::move(newName); }
    
    protected:
        ObjectBase* parent;
        std::string name;
    };
}
