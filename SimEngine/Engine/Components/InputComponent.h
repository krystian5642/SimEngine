#pragma once

#include "Component.h"

namespace SimEngine
{
    struct InputData
    {
        float deltaTime;
        double mouseXDelta;
        double mouseYDelta;
    };

    class InputActionBase
    {
    public:
        virtual ~InputActionBase() = default;
        virtual void Execute(const InputData& inputData) = 0;
    };

    template <class T>
    class InputAction : public InputActionBase
    {
    public:
        InputAction(T* obj, void (T::*func)(const InputData&))
            : obj(obj), func(func)
        {}
    
        void Execute(const InputData& inputData) override
        {
            (obj->*func)(inputData);
        }
    
    private:
        T* obj = nullptr;
        void(T::*func)(const InputData&);
    };

    class InputComponent : public Component
    {
    public:
        InputComponent(ObjectBase* parent, Scene* scene, const std::string& name);
        ~InputComponent() override;
    
        void Tick(float deltaTime) override;
    
        template <class T>
        void AssignAction(int key, T* obj, void(T::*func)(const InputData&));
    
        template <class T>
        void AssingOnMouseMove(T* obj, void(T::*func)(const InputData&));
    
    protected:
        std::unordered_map<int, std::unique_ptr<InputActionBase>> keyToActionMap;
        std::unique_ptr<InputActionBase> onMouseMoveAction;
    };

    template <class T>
    void InputComponent::AssignAction(int key, T* obj, void(T::*func)(const InputData&))
    {
        keyToActionMap[key] = std::make_unique<InputAction<T>>(obj, func);
    }

    template <class T>
    void InputComponent::AssingOnMouseMove(T* obj, void(T::* func)(const InputData&))
    {
        onMouseMoveAction = std::make_unique<InputAction<T>>(obj, func);
    }
}
