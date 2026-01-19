#include "App.h"

#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include "Logging/Log.h"
#include "Rendering/Renderer/OpenGL/OpenGLRenderer.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

App::App()
{
    currentWindow = &window;
}

void App::Run()
{
    Log::Init();
    Renderer::InitStatic(std::make_unique<OpenGLRenderer>());
    SceneManager::Init();
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window.GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    lastFrameTime = glfwGetTime();
    while (!window.ShouldClose())
    {
        double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;
        
        window.Update();
        if (glfwGetWindowAttrib(window.GetGLFWWindow(), GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        const double tickTime1 = glfwGetTime();
        SceneManager::Tick(deltaTime);
        const double tickTime2 = glfwGetTime();
        
        const double renderTime1 = glfwGetTime();
        SceneManager::Render();
        const double renderTime2 = glfwGetTime();
        
        ImGui::Begin("Menu");
        
        ImGui::Text("Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Tick Time: %.3f ms", (tickTime2 - tickTime1) * 1000.0f);
        ImGui::Text("Render Time: %.3f ms", (renderTime2 - renderTime1) * 1000.0f);
        ImGui::Text("Total time: %.3f ms", (renderTime2 - tickTime1) * 1000.0f);
        
        ImGui::Separator();
        if (ImGui::Button(isPaused ? "Resume" : "Pause"))
        {
            isPaused = !isPaused;
        }
        
        ImGui::Separator();
        ImGui::Text("Scenes");
        const auto scenes = SceneManager::GetSceneNames();
        for (const auto& scene : scenes)
        {
            if (ImGui::Button(scene.c_str()))
            {
                SceneManager::LoadScene(scene);
            }
        }
        
        ImGui::End(); 
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        window.SwapBuffers();
    }
    
    SceneManager::OnDestroy();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
}
