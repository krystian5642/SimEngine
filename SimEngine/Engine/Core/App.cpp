#include "App.h"

#include "Logging/Log.h"
#include "Rendering/Renderer/OpenGL/OpenGLRenderer.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

namespace SimEngine
{
    App::App()
    {
        currentWindow = &window;
    }

    App::~App()
    {
    }

    void App::Run()
    {
        //Log::Init();
        Renderer::InitStatic(std::make_unique<OpenGLRenderer>());
        SceneManager::Init();

        lastFrameTime = glfwGetTime();
        while (!window.ShouldClose())
        {
            double currentFrameTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
            lastFrameTime = currentFrameTime;

            const std::string title = "Sim Engine | FPS: " + std::to_string(static_cast<int>(1.0f / deltaTime));
            window.SetWindowTitle(title);

            window.Update();
            glStencilMask(0x00);

            SceneManager::Tick(deltaTime);
            SceneManager::Render(window);

            window.SwapBuffers();
        }
        
        SceneManager::OnDestroy();

        glfwTerminate();   
    }
}
