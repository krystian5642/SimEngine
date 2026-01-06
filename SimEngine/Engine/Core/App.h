#pragma once

#include "Window.h"

namespace SimEngine
{
    class App;
}

extern SimEngine::App* CreateApplication();

namespace SimEngine
{
    class Scene;
    
    class App
    {
    public:
        App();
        virtual ~App() = 0;
    
        void Run();
        
        static Window* GetCurrentWindow() { return currentWindow; }
    
    private:
        Window window{1920, 1080, "Sim Engine"};

        double lastFrameTime{};
        
        static inline Window* currentWindow;
        
        double lastSetWindowTitleTime{};
        double setWindowTitleInterval = 0.2;
    };
}
