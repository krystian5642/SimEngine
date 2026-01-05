#include "InputComponent.h"

#include "Core/App.h"

namespace SimEngine
{
    InputComponent::InputComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : Component(parent, scene, name)
    {
    }

    InputComponent::~InputComponent()
    {
    }

    void InputComponent::Tick(float deltaTime)
    {
        auto* window = App::GetCurrentWindow();
        for (auto& [key, inputAction] : keyToActionMap)
        {
            if (window->IsKeyDown(key))
            {
                InputData inputData{};
                inputData.deltaTime = deltaTime;
                inputAction->Execute(inputData);
            }
        }
    
        if (onMouseMoveAction)
        {
            const double xDelta = window->ConsumeXMouseDelta();
            const double yDelta = window->ConsumeYMouseDelta();
     
            const InputData inputData{deltaTime, xDelta, yDelta};
            onMouseMoveAction->Execute(inputData);
        }
    }
}
