#pragma once

#include "Window.h"
#include "Rendering/Core/Renderer.h"

class Scene;

class App
{
public:
    void Run();
    
    static App& Get();
    
    Window window{1920, 1080, "Sim Engine"};
    Renderer renderer;
    
    bool isPaused{false};

private:
    App();
    
    double lastFrameTime{};
};
