#pragma once

#include "Window.h"
#include "Rendering/Renderer/Renderer.h"

class App;

extern App* CreateApplication();

class Scene;

class App
{
public:
    App();
    virtual ~App() = 0 {}

    void Run();
    
    static Window* GetCurrentWindow() { return currentWindow; }
    
    bool GetIsPaused() const { return isPaused; }
    
    static inline App* currentApp = nullptr;
    
protected:
    bool isPaused{false};

private:
    Window window{1920, 1080, "Sim Engine"};
    Renderer renderer;

    double lastFrameTime{};
    
    static inline Window* currentWindow;
};
